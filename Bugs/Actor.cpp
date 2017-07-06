#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Compiler.h"
#include <string>
#include <cmath>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//////////////////////////////////////////
//                 ACTOR                //
//////////////////////////////////////////

Actor::~Actor() {}

//////////////////////////////////////////
//            ENERGY HOLDER             //
//////////////////////////////////////////

EnergyHolder::~EnergyHolder() {}

void EnergyHolder::gainEnergy(int amt) {
    if (amt <= 0)
        return;
    m_energy+=amt;
}

void EnergyHolder::loseEnergy(int amt) {
    m_energy-=amt;
    if (m_energy <= 0)
        setDead();
}

//////////////////////////////////////////
//                 EATER                //
//////////////////////////////////////////

Eater::~Eater() {}

int Eater::Eat(int amt) {
    int res = getWorld()->eatOrPickUpFood(getX(), getY(), amt);
    gainEnergy(res);
    return res;
}

//////////////////////////////////////////
//                 INSECT               //
//////////////////////////////////////////

Insect::Insect(int imageID, int startX, int startY, StudentWorld* world, int energy) :
Eater(imageID, startX, startY, 1, world, energy) {
    
    m_SleepOrStunTicks = 0;
    m_prevStunned = false;
    
    /* Insect starts facing a random direction */
    FaceRandDir();
}

void Insect::FaceRandDir() {
    int rnd = randInt(0, 3);
    switch (rnd) {
        case 0:
            setDirection(up);
            break;
        case 1:
            setDirection(right);
            break;
        case 2:
            setDirection(down);
            break;
        case 3:
            setDirection(left);
            break;
    }
}

bool Insect::moveInDir(GraphObject::Direction dir) {
    
    switch (dir) {   //Try to move
        case right:
            if (!getWorld()->isSquareWalkable(getX()+1, getY()))
                return false;
            moveTo(getX()+1, getY());
            m_prevStunned = false;               //not stunned yet on new square
            break;
        case left:
            if (!getWorld()->isSquareWalkable(getX()-1, getY()))
                return false;
            moveTo(getX()-1, getY());
            m_prevStunned = false;
            break;
        case up:
            if (!getWorld()->isSquareWalkable(getX(), getY()+1))
                return false;
            moveTo(getX(), getY()+1);
            m_prevStunned = false;
            break;
        case down:
            if (!getWorld()->isSquareWalkable(getX(), getY()-1))
                return false;
            moveTo(getX(), getY()-1);
            m_prevStunned = false;
            break;
        default:
            break;
    }

    return true;
}

void Insect::Die() {
    getWorld()->addOrCreateFood(getX(), getY(), 100);
    setDead();
}

void Insect::getBitten(int amt) {
    loseEnergy(amt);
}

void Insect::getStunned() {
    if (canIBeStunned() && !m_prevStunned) {
        increaseSleepOrStunTicks(2);
        m_prevStunned = true;
    }
}

void Insect::getPoisoned() {
    if (canIBePoisoned())
        loseEnergy(150);
}

int Insect::initialInsectActions() {
    
    loseEnergy(1);
    if (getEnergy() <= 0) {
        Die();
        return 0;
    }
    
    if (isSleepingOrStunned()) {
        m_SleepOrStunTicks--;
        return 0;
    }
    
    return 2;
}

//////////////////////////////////////////
//             GRASSHOPPER              //
//////////////////////////////////////////

void Grasshopper::doSomething() {
    
    int res = initialInsectActions();      //Do what all Insects do
    
    if (res != 2)                          //Grasshopper died or is sleeping/stunned
        return;
    
    res = preEatActions();                 //Do what specific derived classes do differently
    
    if (res == 0)                          //BabyGrasshopper turned into AdultGrasshopper
        return;
    
    if (res == 1) {                        //AdultGrasshopper bit enemy or jumped
        increaseSleepOrStunTicks(2);
        return;
    }
    
    postEatActions();                      //Do what all derived classes do
}

/* private member functions */

void Grasshopper::postEatActions() {
    if(Eat(200) > 0) {    // makes no changes if no Food present
        if(randInt(0, 1))
            goto sleep;
    }
    
    if (remainingMoveDist() == 0) { //Pick random directon and distance
        FaceRandDir();
        SetRandDist();
    }
    
    if(!moveInDir(getDirection())) {    //Try to move
        m_desiredMoveDist = 0;
        goto sleep;
    }
    
    m_desiredMoveDist--;
    
sleep:
    increaseSleepOrStunTicks(2);
}

//////////////////////////////////////////
//          BABY GRASSHOPPER            //
//////////////////////////////////////////

/* private member functions */
int BabyGrasshopper::preEatActions() {
    if (getEnergy() >= 1600) {
        getWorld()->createAdultGrasshopper(getX(), getY()); //turn into AdultGrasshopper
        Die();
        return 0;
    }
    return 2;
}

//////////////////////////////////////////
//          ADULT GRASSHOPPER           //
//////////////////////////////////////////

void AdultGrasshopper::getBitten(int amt) {
    Insect::getBitten(amt);
    if (getEnergy() > 0 && randInt(0, 1) == 1)
        biteRandomEnemy();
}

/* private member functions */

bool AdultGrasshopper::biteRandomEnemy() {
    return getWorld()->biteRandAdultGrasshopperEnemy(this);
}

int AdultGrasshopper::preEatActions() {
    
    if (randInt(0, 2) == 0 && biteRandomEnemy())
        return 1;
    
    if (randInt(1, 10) == 1 && jumpToRandSquare())
        return 1;
    
    return 2;
}

bool AdultGrasshopper::jumpToRandSquare() {
    int openSquareCount = 0;
    
    for (int i = 0; i < VIEW_WIDTH; i++) {
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            if ( pow((i-getX()), 2) +  pow((j-getY()), 2) <= 100 && getWorld()->isSquareWalkable(i, j)) {
                
                if (i != getX() || j != getY()) // Doesn't want to jump to same square
                    openSquareCount++;
            }
        }
    }
    
    if (openSquareCount==0)
        return false;
    
    int rand = randInt(1, openSquareCount);
    int cnt = 0;
    
    for (int i = 0; i < VIEW_WIDTH; i++) {
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            if ( pow((i-getX()), 2) +  pow((j-getY()), 2) <= 100 && getWorld()->isSquareWalkable(i, j)) {
                
                if (i != getX() || j != getY()) // Doesn't want to jump to same square
                    cnt++;
            
                if ( cnt == rand )
                    moveTo(i, j);
            }
        }
    }
    
    return true;
}

//////////////////////////////////////////
//                 ANT                  //
//////////////////////////////////////////

int Ant::Eat(int amt) {
    
    if (m_foodUnitsHeld >= amt) {
        gainEnergy(amt);
        m_foodUnitsHeld -= amt;
        return amt;
    }
    
    gainEnergy(m_foodUnitsHeld);
    int ret = m_foodUnitsHeld;
    m_foodUnitsHeld = 0;
    
    return ret;
}

void Ant::getBitten(int amt) {
    Insect::getBitten(amt);
    setPreviouslyBitten();
}

void Ant::doSomething() {
    
    if(Insect::initialInsectActions() != 2) // died or currently stunned
        return;
    
    int count = 0;
    Compiler::Command cmd;
    
    while (count <= 10) {
        
        if (!m_Compiler->getCommand(m_instructionCounter, cmd)) {
            setDead();
            return;
        }
        
        if (interpreter(cmd) == 1)
            return;
        else 
            count++;
    }
    
}

/* private member functions */

bool Ant::biteRandomEnemy() {
    return getWorld()->biteRandAntEnemy(this);
}

void Ant::rotateClockwise() {
    if (getDirection() == right)
        setDirection(down);
    else if (getDirection() == down)
        setDirection(left);
    else if (getDirection() == left)
        setDirection(up);
    else if (getDirection() == up)
        setDirection(right);
}

void Ant::rotateCounterClockwise() {
    if (getDirection() == right)
        setDirection(up);
    else if (getDirection() == up)
        setDirection(left);
    else if (getDirection() == left)
        setDirection(down);
    else if (getDirection() == down)
        setDirection(right);
}

void Ant::dropFood() {
    getWorld()->addOrCreateFood(getX(), getY(), m_foodUnitsHeld);   //does nothing if no food is held
    m_foodUnitsHeld = 0;
}

int Ant::pickUpFood() {
    
    int units = 0;
    
    if (m_foodUnitsHeld <= 1400) {
        units = getWorld()->eatOrPickUpFood(getX(), getY(), 400);
        m_foodUnitsHeld+=units;
        return units;
    }
    
    units = getWorld()->eatOrPickUpFood(getX(), getY(), 1800-m_foodUnitsHeld);
    m_foodUnitsHeld+=units;
    return units;
}

int Ant::interpreter(Compiler::Command &cmd) {
    
    switch (cmd.opcode) {
            
        case Compiler::moveForward:
            
            if(!moveInDir(getDirection()))
                setPreviouslyBlocked();
            else {
                setNotPreviouslyBlocked();
                setNotPreviouslyBitten();
            }
            m_instructionCounter++;
            return 1;
            
        case Compiler::eatFood:
            Eat(100);
            m_instructionCounter++;
            return 1;
            
        case Compiler::dropFood:
            dropFood();
            m_instructionCounter++;
            return 1;
            
        case Compiler::bite:
            biteRandomEnemy();
            m_instructionCounter++;
            return 1;
            
        case Compiler::pickupFood:
            pickUpFood();
            m_instructionCounter++;
            return 1;
            
        case Compiler::emitPheromone:
            getWorld()->addOrCreatePheromone(getX(), getY(), m_colonyNum);
            m_instructionCounter++;
            return 1;
            
        case Compiler::faceRandomDirection:
            FaceRandDir();
            m_instructionCounter++;
            return 1;
            
        case Compiler::rotateClockwise:
            rotateClockwise();
            m_instructionCounter++;
            return 1;
            
        case Compiler::rotateCounterClockwise:
            rotateCounterClockwise();
            m_instructionCounter++;
            return 1;
            
        case Compiler::generateRandomNumber: {
            int upper = stoi(cmd.operand1);
            if (upper == 0)
                m_lastRandNum = 0;
            else
                m_lastRandNum = randInt(0, upper);
            m_instructionCounter++;
            return 0;
        }
            
        case Compiler::goto_command:
            m_instructionCounter = stoi(cmd.operand1);
            return 0;
            
        case Compiler::if_command:
            if (commandTriggered(cmd))
                m_instructionCounter = stoi(cmd.operand2);
            else
                m_instructionCounter++;
            return 0;
            
        default:
            return 0;
    }
}

bool Ant::commandTriggered(Compiler::Command &cmd) const {
    
    switch (stoi(cmd.operand1)) {
            
        case 0:                                             //i_smell_danger_in_front_of_me
            return getWorld()->DangerAheadOfAnt(this);
            
        case 1:
            return getWorld()->PheromoneAheadOfAnt(this);   //i_smell_pheromone_in_front_of_me
            
        case 2:                                             //i_was_bit
            return wasPreviouslyBitten();
            
        case 3:                                             //i_am_carrying_food
            return m_foodUnitsHeld > 0;
            
        case 4:                                             //i_am_hungry
            return getEnergy() <= 25;
            
        case 5:                                             //i_am_standing_on_my_anthill
            return (getX()==getWorld()->getColonyX(getColony()) && getY()==getWorld()->getColonyY(getColony()));
        
        case 6:                                             //i_am_standing_on_food
            return getWorld()->checkEatable(getX(), getY());
            
        case 7:                                             //i_am_standing_with_an_enemy
            return getWorld()->countAntEnemies(this) > 0;
            
        case 8:                                             //i_was_blocked_from_moving
            return wasPreviouslyBlocked();
            
        case 9:                                             //last_random_number_was_zero
            return m_lastRandNum==0;
            
        default:
            break;
    }
    return false;
}

//////////////////////////////////////////
//                ANTHILL               //
//////////////////////////////////////////

void Anthill::doSomething() {
    loseEnergy(1);
    if (isDead())
        return;
    if (Eat(10000)>0)
        return;
    createAnt();            //does nothing if energy < 2000
}

/* private member functions */

void Anthill::createAnt() {
    if (getEnergy() < 2000)
        return;
    switch (m_colonyNum) {
        case 0:
            getWorld()->addAnt(new Ant(IID_ANT_TYPE0, getX(), getY(), 0, getWorld(), m_Compiler));
            break;
            
        case 1:
            getWorld()->addAnt(new Ant(IID_ANT_TYPE1, getX(), getY(), 1, getWorld(), m_Compiler));
            break;
            
        case 2:
            getWorld()->addAnt(new Ant(IID_ANT_TYPE2, getX(), getY(), 2, getWorld(), m_Compiler));
            break;
            
        case 3:
            getWorld()->addAnt(new Ant(IID_ANT_TYPE3, getX(), getY(), 3, getWorld(), m_Compiler));
            break;
            
        default:
            break;
    }
    loseEnergy(1500);
}

//////////////////////////////////////////
//                 POISON               //
//////////////////////////////////////////

/* private member functions */

void Poison::impactActors() {
    getWorld()->poisonActorsAt(getX(), getY());
}

//////////////////////////////////////////
//             POOL OF WATER            //
//////////////////////////////////////////

/* private member functions */

void PoolOfWater::impactActors() {
    getWorld()->stunActorsAt(getX(), getY());
}