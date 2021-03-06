/*
 *  ReaperTests.cpp
 *  MacTierra
 *
 *  Created by Simon Fraser on 8/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ReaperTests.h"

#include <iostream>

#include "MT_Creature.h"
#include "MT_Reaper.h"
#include "MT_Soup.h"


using namespace MacTierra;


ReaperTests::ReaperTests()
: mSoup(NULL)
, mReaper(NULL)
{
}


ReaperTests::~ReaperTests()
{
}

void
ReaperTests::setUp()
{
    mSoup = new Soup(1024);
    mReaper = new Reaper();
}

void
ReaperTests::tearDown()
{
    delete mSoup; mSoup = NULL;
    delete mReaper; mReaper = NULL;
}

void
ReaperTests::runTest()
{
    std::cout << "ReaperTests" << std::endl;

    creature_id creatureID = 100;
    RefPtr<Creature>   creature1 = Creature::create(++creatureID, 100, mSoup);
    RefPtr<Creature>   creature2 = Creature::create(++creatureID, 100, mSoup);

    creature1->setNumErrors(2);
    creature2->setNumErrors(4);
    
    // test empty list
    Creature* head = mReaper->headCreature();
    TEST_CONDITION(!head);
    
    TEST_CONDITION(!mReaper->conditionalMoveUp(*creature1));
    TEST_CONDITION(!mReaper->conditionalMoveDown(*creature2));

    // add to list
    mReaper->addCreature(*creature1);
    mReaper->addCreature(*creature2);
    
    mReaper->printCreatures();
    
    TEST_CONDITION(mReaper->numberOfCreatures() == 2);
    TEST_CONDITION(mReaper->headCreature() == creature1);
    
    TEST_CONDITION(mReaper->conditionalMoveUp(*creature2));
    TEST_CONDITION(mReaper->headCreature() == creature2);

    creature2->setNumErrors(1);

    TEST_CONDITION(mReaper->conditionalMoveDown(*creature2));
    TEST_CONDITION(mReaper->headCreature() == creature1);

    mReaper->removeCreature(*creature1);
    TEST_CONDITION(mReaper->headCreature() == creature2);

    mReaper->printCreatures();

    mReaper->removeCreature(*creature2);
    
}

TestRegistration reaperTestReg(new ReaperTests);
