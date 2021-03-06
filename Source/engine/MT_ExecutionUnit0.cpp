/*
 *  MT_ExecutionUnit0.cpp
 *  MacTierra
 *
 *  Created by Simon Fraser on 8/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "MT_ExecutionUnit0.h"

#include "MT_Creature.h"
#include "MT_Cpu.h"
#include "MT_Isa.h"
#include "MT_InstructionSet.h"
#include "MT_Soup.h"
#include "MT_World.h"

namespace MacTierra {

using namespace std;

ExecutionUnit0::ExecutionUnit0()
{
}

ExecutionUnit0::~ExecutionUnit0()
{
}

PassRefPtr<Creature>
ExecutionUnit0::execute(Creature& inCreature, World& inWorld, int32_t inFlaw)
{
    PassRefPtr<Creature> resultCreature;

    Cpu& cpu = inCreature.cpu();
    cpu.clearFlag();
    
    if (inWorld.settings().selectForLeanness())
    {
        const int32_t ip = cpu.mInstructionPointer;
        if (ip >= 0 && ip < inCreature.length())
            inCreature.setExecutedBit(ip);
    }
    
    instruction_t   theInst = inCreature.getSoupInstruction(cpu.mInstructionPointer);
    
    //cout << "Executing instruction " << (int32_t)theInst << endl;
    
    switch (theInst)
    {
        case k_nop_0:
        case k_nop_1:
            // do nothing
            break;

        case k_or1:     // Flip the low order bit of cx
            cpu.mRegisters[k_cx] = cpu.mRegisters[k_cx] ^ (1 + inFlaw);
            break;
    
        case k_sh1:     // Shift cx left 1
            cpu.mRegisters[k_cx] = cpu.mRegisters[k_cx] << (1 + inFlaw);
            break;
    
        case k_zero:    // Zero cx
            cpu.mRegisters[k_cx + inFlaw] = 0;
            break;
    
        case k_if_cz:   // If cx=0 execute next instruction
            if (cpu.mRegisters[k_cx + inFlaw] != 0)
                cpu.incrementIP(inWorld.soupSize());
            break;
    
        case k_sub_ab:  // Subtract bx from ax->cx
            cpu.mRegisters[k_cx + inFlaw] = cpu.mRegisters[k_ax] - cpu.mRegisters[k_bx];
            break;
    
        case k_sub_ac:  // Subtract ax - cx->ax
            {
                int32_t targetRegister = (k_ax + kNumRegisters + inFlaw) % kNumRegisters;
                cpu.mRegisters[targetRegister] = cpu.mRegisters[targetRegister] - cpu.mRegisters[k_cx];
            }
            break;

        case k_inc_a:   // Increment ax
            cpu.mRegisters[k_ax] = cpu.mRegisters[k_ax] + 1 + inFlaw;
            break;

        case k_inc_b:   // Increment bx
            cpu.mRegisters[k_bx] = cpu.mRegisters[k_bx] + 1 + inFlaw;
            break;

        case k_dec_c:   // Decrement cx
            cpu.mRegisters[k_cx] = cpu.mRegisters[k_cx] - 1 + inFlaw;
            break;

        case k_inc_c:   // Increment cx
            cpu.mRegisters[k_cx] = cpu.mRegisters[k_cx] + 1 + inFlaw;
            break;

        case k_push_ax: // Push ax onto stack
            {
                int32_t targetRegister = (k_ax + kNumRegisters + inFlaw) % kNumRegisters; 
                cpu.push(cpu.mRegisters[targetRegister]);
            }
            break;

        case k_push_bx: // Push bx onto stack
            {
                int32_t targetRegister = k_bx + inFlaw; 
                cpu.push(cpu.mRegisters[targetRegister]);
            }
            break;

        case k_push_cx: // Push cx onto stack
            {
                int32_t targetRegister = k_cx + inFlaw; 
                cpu.push(cpu.mRegisters[targetRegister]);
            }
            break;

        case k_push_dx: // Push dx onto stack
            {
                int32_t targetRegister = (k_dx + inFlaw) % kNumRegisters; 
                cpu.push(cpu.mRegisters[targetRegister]);
            }
            break;

        case k_pop_ax:  // Pop top of stack into ax
            {
                int32_t targetRegister = (k_ax + kNumRegisters + inFlaw) % kNumRegisters; 
                cpu.mRegisters[targetRegister] = cpu.pop();
            }
            break;

        case k_pop_bx:  // Pop top of stack into bx
            {
                int32_t targetRegister = k_bx + inFlaw; 
                cpu.mRegisters[targetRegister] = cpu.pop();
            }
            break;

        case k_pop_cx:  // Pop top of stack into cx
            {
                int32_t targetRegister = k_cx + inFlaw; 
                cpu.mRegisters[targetRegister] = cpu.pop();
            }
            break;

        case k_pop_dx:  // Pop top of stack into dx
            {
                int32_t targetRegister = (k_dx + inFlaw) % kNumRegisters; 
                cpu.mRegisters[targetRegister] = cpu.pop();
            }
            break;
    
        case k_jmp:     // Jump (search for template pointed to by the IP)
            jump(inCreature, *inWorld.soup(), Soup::kBothways);
            break;

        case k_jumpb:   // Jump backwards
            jump(inCreature, *inWorld.soup(), Soup::kBackwards);
            break;

        case k_call:    // Call
            call(inCreature, *inWorld.soup());
            break;
            
        case k_ret:     // Return
            // get ip from top of stack; -1 because it's incremented later
            cpu.mInstructionPointer = cpu.pop() - 1;
            break;

        case k_mov_cd:  // Copy cx into dx
            cpu.mRegisters[(k_dx + inFlaw) % kNumRegisters] = cpu.mRegisters[k_cx + inFlaw];
            break;

        case k_mov_ab:  // Copy ax into bx
            cpu.mRegisters[k_bx + inFlaw] = cpu.mRegisters[(k_ax + kNumRegisters + inFlaw) % kNumRegisters];
            break;
            
        case k_mov_iab: // Copy inst at address in bx to address in ax
            {
                instruction_t   inst = inCreature.getSoupInstruction(cpu.mRegisters[k_bx]);
                u_int32_t       soupSize = inWorld.soupSize();
                address_t       targetAddress = inCreature.addressFromOffset(cpu.mRegisters[k_ax]);
                
                if (inWorld.copyErrorPending())
                    inst = inWorld.mutateInstruction(inst, inWorld.settings().mutationType());
                
                if (inWorld.settings().globalWritesAllowed() || 
                    inCreature.containsAddress(targetAddress, soupSize) || 
                    (inCreature.isDividing() && inCreature.daughterCreature()->containsAddress(targetAddress, soupSize)))
                {
                    inWorld.soup()->setInstructionAtAddress(targetAddress, inst);
                    if (inCreature.isDividing())
                        inCreature.noteMoveToOffspring(targetAddress);
                }
                else
                    cpu.setFlag();
            }
            break;

        case k_adr:     // Addr
            address(inCreature, *inWorld.soup(), Soup::kBothways);
            break;

        case k_adrb:    // AddrBack
            address(inCreature, *inWorld.soup(), Soup::kBackwards);
            break;

        case k_adrf:    // AddrFor
            address(inCreature, *inWorld.soup(), Soup::kForwards);
            break;
            
        case k_mal:     // Allocate
            memoryAllocate(inCreature, inWorld);
            break;
            
        case k_divide:  // Divide
            resultCreature = inCreature.divide(inWorld);
            break;
    }

    cpu.incrementIP(inWorld.soupSize());

    // Remember errors for the grim reaper
    inCreature.noteErrors();
    inCreature.executedInstruction(theInst);
    
    return resultCreature;
}

#pragma mark -

void
ExecutionUnit0::memoryAllocate(Creature& inCreature, World& inWorld)
{
    Cpu& cpu = inCreature.cpu();

    u_int32_t daughterLength = cpu.mRegisters[k_cx];
    if (inCreature.isDividing() ||
        daughterLength < kMinCreatureSize ||
        (daughterLength > inCreature.length() * kMaxDaughterSizeMultiple))
    {
        cpu.setFlag();
        return;
    }
    
    RefPtr<Creature> daughter = inWorld.allocateSpaceForOffspring(inCreature, daughterLength);
    if (daughter)
    {
        cpu.mRegisters[k_ax] = inCreature.offsetFromAddress(daughter->location());
        inCreature.startDividing(daughter.get());

        if (inWorld.settings().clearDaughterCells())
            daughter->clearSpace();
    }
    else
    {
        cpu.setFlag();
    }

}

void
ExecutionUnit0::jump(Creature& inCreature, Soup& inSoup, Soup::ESearchDirection inDirection)
{
    Cpu& cpu = inCreature.cpu();

    u_int32_t templateLength = 0;
    u_int32_t templateOffset = inCreature.referencedLocation() + 1;

    if (inSoup.seachForTemplate(inDirection, templateOffset, templateLength))
    {
        // subtract one so that when we increment the IP later it points to the found location
        u_int32_t soupSize = inSoup.soupSize();
        inCreature.setReferencedLocation((templateOffset + templateLength + soupSize - 1) % soupSize);
    }
    else
    {
        //cout << "Failed to find jump template" << endl;
        cpu.mInstructionPointer += templateLength;
        cpu.setFlag();
    }
}

void
ExecutionUnit0::call(Creature& inCreature, Soup& inSoup)
{
    Cpu& cpu = inCreature.cpu();

    u_int32_t templateLength = 0;
    u_int32_t templateOffset = inCreature.referencedLocation() + 1;

    if (inSoup.seachForTemplate(Soup::kBothways, templateOffset, templateLength))
    {
        cpu.push(cpu.mInstructionPointer + templateLength + 1); // FIXME: verify the + 1
        
        // subtract one so that when we increment the IP later it points to the found location
        u_int32_t soupSize = inSoup.soupSize();
        inCreature.setReferencedLocation((templateOffset + templateLength + soupSize - 1) % soupSize);
    }
    else
    {
        //cout << "Failed to find call template" << endl;
        if (templateLength == 0)
        {
            cpu.push(cpu.mInstructionPointer + 1);
            // tierra doesn't set the flag here, apparently
        }
        else
        {
            cpu.mInstructionPointer += templateLength;
            cpu.setFlag();
        }
    }
}

void
ExecutionUnit0::address(Creature& inCreature, Soup& inSoup, Soup::ESearchDirection inDirection)
{
    Cpu& cpu = inCreature.cpu();

    u_int32_t templateLength = 0;
    address_t templateOffset = inCreature.referencedLocation() + 1;

    if (inSoup.seachForTemplate(inDirection, templateOffset, templateLength))
    {
        cpu.mRegisters[k_ax] = inCreature.offsetFromAddress(templateOffset + templateLength);
        // Step past template
        cpu.mInstructionPointer += templateLength;
        cpu.mRegisters[k_cx] = templateLength;
    }
    else
    {
        //cout << "Failed to find address template" << endl;
        cpu.setFlag();
    }
}


} // namespace MacTierra

