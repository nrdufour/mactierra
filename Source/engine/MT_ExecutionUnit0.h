/*
 *  MT_ExecutionUnit0.h
 *  MacTierra
 *
 *  Created by Simon Fraser on 8/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef MT_ExecutionUnit0_h
#define MT_ExecutionUnit0_h

#include "MT_ExecutionUnit.h"

#include "MT_Soup.h"

namespace MacTierra {

class Creature;
class Cpu;

// Execution unit for instruction set 0
class ExecutionUnit0 : public ExecutionUnit
{
public:

    ExecutionUnit0();
    ~ExecutionUnit0();
    
    virtual Creature* execute(Creature& inCreature, World& inWorld, int32_t inFlaw);


protected:

    void memoryAllocate(Creature& inCreature, World& inWorld);

    void jump(Creature& inCreature, Soup& inSoup, Soup::ESearchDirection inDirection);
    void call(Creature& inCreature, Soup& inSoup);
    void address(Creature& inCreature, Soup& inSoup, Soup::ESearchDirection inDirection);

};


} // namespace MacTierra


#endif // MT_ExecutionUnit0_h