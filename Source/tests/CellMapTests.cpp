/*
 *  CellMapTests.cpp
 *  MacTierra
 *
 *  Created by Simon Fraser on 8/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "CellMapTests.h"


#include <iostream>

#include "mt_creature.h"
#include "mt_cellmap.h"
#include "mt_world.h"


using namespace MacTierra;


CellMapTests::CellMapTests()
: mWorld(NULL)
{
}


CellMapTests::~CellMapTests()
{
}

void
CellMapTests::setUp()
{
    mWorld = new World();
    mWorld->initializeSoup(1024);

}

void
CellMapTests::tearDown()
{
    delete mWorld; mWorld = NULL;
}

void
CellMapTests::runTest()
{
    std::cout << "CellMapTests" << std::endl;

    Creature*   creature1 = mWorld->createCreature();
    creature1->setLocation(100);
    creature1->setLength(100);

    Creature*   creature2 = mWorld->createCreature();
    creature2->setLocation(200);
    creature2->setLength(100);

    Creature*   creature3 = mWorld->createCreature();
    creature3->setLocation(400);
    creature3->setLength(100);

    // wrapping creature
    Creature*   creature4 = mWorld->createCreature();
    creature4->setLocation(1000);
    creature4->setLength(100);

    TEST_CONDITION(creature1 && creature2 && creature3);

    CellMap*    cellMap = mWorld->cellMap();
    TEST_CONDITION(cellMap);

    // test empty map
    size_t foundIndex = 0;
    TEST_CONDITION(!cellMap->indexOfCreatureAtAddress(0, foundIndex));

    TEST_CONDITION(!cellMap->creatureAtAddress(0));
    TEST_CONDITION(!cellMap->creatureAtAddress(100));

    TEST_CONDITION(cellMap->insertCreature(creature1));
    cellMap->printCreatures();

    TEST_CONDITION(!cellMap->indexOfCreatureAtAddress(99, foundIndex));
    TEST_CONDITION(cellMap->indexOfCreatureAtAddress(100, foundIndex) && foundIndex == 0);
    TEST_CONDITION(cellMap->indexOfCreatureAtAddress(101, foundIndex) && foundIndex == 0);
    TEST_CONDITION(cellMap->indexOfCreatureAtAddress(199, foundIndex) && foundIndex == 0);
    TEST_CONDITION(!cellMap->indexOfCreatureAtAddress(200, foundIndex));
    TEST_CONDITION(!cellMap->indexOfCreatureAtAddress(201, foundIndex));
    TEST_CONDITION(!cellMap->indexOfCreatureAtAddress(1023, foundIndex));

    TEST_CONDITION(cellMap->spaceAtAddress(1020, 6));

    cellMap->printCreatures();

    TEST_CONDITION(!cellMap->creatureAtAddress(0));
    TEST_CONDITION(!cellMap->creatureAtAddress(99));
    TEST_CONDITION(cellMap->creatureAtAddress(100));
    TEST_CONDITION(cellMap->creatureAtAddress(199));
    TEST_CONDITION(!cellMap->creatureAtAddress(200));
    TEST_CONDITION(!cellMap->creatureAtAddress(1023));

    TEST_CONDITION(cellMap->insertCreature(creature4));

    cellMap->printCreatures();

    TEST_CONDITION(cellMap->insertCreature(creature3));
    TEST_CONDITION(cellMap->insertCreature(creature2));

    cellMap->printCreatures();

    TEST_CONDITION(cellMap->creatureAtAddress(1023));
    TEST_CONDITION(cellMap->creatureAtAddress(0));
    TEST_CONDITION(cellMap->creatureAtAddress(75));
    TEST_CONDITION(!cellMap->creatureAtAddress(76));

    TEST_CONDITION(cellMap->spaceAtAddress(300, 100));
    TEST_CONDITION(!cellMap->spaceAtAddress(300, 101));
    TEST_CONDITION(!cellMap->spaceAtAddress(98, 108));
    TEST_CONDITION(!cellMap->spaceAtAddress(104, 90));
    TEST_CONDITION(!cellMap->spaceAtAddress(1020, 6));
    TEST_CONDITION(!cellMap->spaceAtAddress(2, 4));

}

TestRegistration cellMapTestReg(new CellMapTests);
