//
//  MTCreature.mm
//  MacTierra
//
//  Created by Simon Fraser on 8/20/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "MTCreature.h"

#import "MT_Engine.h"
#import "MT_Creature.h"
#import "MT_InstructionSet.h"
#import "MT_ISA.h"

#import "MTInventoryGenotype.h"

NSString* const kCreaturePasteboardType = @"org.smfr.mactierra.creature";

@implementation MTCreature

- (id)initWithCreature:(MacTierra::Creature*)inCreature
{
    if ((self = [super init]))
    {
        mCreature = inCreature;
    }
    return self;
}

- (void)dealloc
{
    [genotype release];
    
    [super dealloc];
}

- (MacTierra::Creature*)creature
{
    return mCreature;
}

- (NSString*)name
{
    return [NSString stringWithUTF8String:mCreature->creatureName().c_str()];
}

- (NSData*)genome
{
    std::string genomeString = mCreature->genomeData().dataString();
    
    return [NSData dataWithBytes:genomeString.data() length:genomeString.length()];
}

- (NSInteger)length
{
    return mCreature->length();
}

- (NSUInteger)location
{
    return mCreature->location();
}

- (NSInteger)instructionPointer
{
    return mCreature->cpu().instructionPointer();
}

- (NSString*)lastInstruction
{
    return [NSString stringWithUTF8String:MacTierra::nameForInstruction(mCreature->lastInstruction())];
}

- (BOOL)flag
{
    return mCreature->cpu().flag();
}

- (NSInteger)axRegister
{
    return mCreature->cpu().registerValue(MacTierra::k_ax);
}

- (NSInteger)bxRegister
{
    return mCreature->cpu().registerValue(MacTierra::k_bx);
}

- (NSInteger)cxRegister
{
    return mCreature->cpu().registerValue(MacTierra::k_cx);
}

- (NSInteger)dxRegister
{
    return mCreature->cpu().registerValue(MacTierra::k_dx);
}

- (NSArray*)stack
{
    NSMutableArray*    stackArray = [NSMutableArray arrayWithCapacity:MacTierra::kStackSize];
    int32_t stackPointer = mCreature->cpu().stackPointer();
    for (u_int32_t i = 0; i < MacTierra::kStackSize; ++i)
    {
        NSDictionary* stackItem = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithInteger:mCreature->cpu().stackValue(i)], @"value",
                                        ((i == stackPointer) ? @"•" : @"") , @"sp",
                                        nil];
        [stackArray addObject:stackItem];
    }
    return stackArray;
}

- (NSArray*)stackPointer
{
    return [NSArray array];
}

- (NSString*)soupAroundIP
{
    // FIXME
    return @"";
}

- (NSRange)soupSelectionRange
{
    return NSMakeRange(0, 1);
}

- (MTInventoryGenotype*)genotype
{
    // FIXME: this will get out of sync with the creature's genotype if that is changed
    if (!genotype)
        genotype = [[MTInventoryGenotype alloc] initWithGenotype:mCreature->genotype()];

    return genotype;
}

@end

#pragma mark -

@implementation MTSerializableCreature

@synthesize name;
@synthesize genome;

- (id)initWithCoder:(NSCoder *)decoder
{
    if ((self = [super init]))
    {
        self.name   = [[decoder decodeObjectForKey:@"name"] retain];
        self.genome = [[decoder decodeObjectForKey:@"genome"] retain];
    }
    return self;
}

- (id)initWithName:(NSString *)inName genome:(NSData*)inGenome
{
    if ((self = [super init]))
    {
        self.name = [[inName copy] autorelease];
        self.genome = [[inGenome copy] autorelease];
    }
    return self;
}

- (void)dealloc
{
    self.name = nil;
    self.genome = nil;
    [super dealloc];
}

- (void)encodeWithCoder:(NSCoder *)encoder
{
    [encoder encodeObject:name forKey:@"name"];
    [encoder encodeObject:genome forKey:@"genome"];
}

@end
