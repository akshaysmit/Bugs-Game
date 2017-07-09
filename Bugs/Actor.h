#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"
using namespace std;

class StudentWorld;

//////////////////////////////////////////
//            LIST OF CLASSES           //
//////////////////////////////////////////

class Actor;
class EnergyHolder;
class Eater;
class Insect;
class Anthill;
class Ant;
class Grasshopper;
class BabyGrasshopper;
class AdultGrasshopper;
class Food;
class Impactor;
class Pebble;
class Poison;
class PoolOfWater;
class Pheromone;

//////////////////////////////////////////
//                 ACTOR                //
//////////////////////////////////////////

class Actor : public GraphObject {
public:
    
    /* constructor/destructor */
    Actor(int imageID, int startX, int startY, int depth, StudentWorld* world) :
    GraphObject(imageID, startX, startY, right, depth), m_world(world), m_is_dead(false), m_has_done_something(false) {}
    
    virtual ~Actor() = 0;   //implemented in Actor.cpp file
    
    /* StudentWorld accessor */
    StudentWorld* getWorld() const { return m_world; }
    
    /* death */
    void setDead() { m_is_dead = true; }
    bool isDead() const { return m_is_dead; }
    
    /* poison */
    virtual void getPoisoned() {};
    
    /* get stunned */
    virtual void getStunned() {};
    
    /* do something */
    void setDoneSomething() { m_has_done_something = true; }
    void setNotDoneSomething() { m_has_done_something = false; }
    bool hasDoneSomething () const { return m_has_done_something; }
    virtual void doSomething() {}
    
    /* Identifier functions */
    virtual bool amIWalkable() const { return true; }
    virtual bool canIBeEaten() const { return false; }
    virtual bool amIEnemyOfAdultGrasshopper() const { return false; }
    virtual bool amIEnemyOfAnt(int colonyNum) { return false; }         //returns true if Actor is enemy of Ant of that colony
    virtual bool amIPheromone() const { return false; }
    virtual bool canIBePoisoned() const { return false; }
    virtual bool canIBeStunned() const { return false; }
    
private:
    StudentWorld* m_world;
    bool m_is_dead;
    bool m_has_done_something;              //used by StudentWorld to check if Actor's doSomething has been called
};

//////////////////////////////////////////
//            ENERGY HOLDER             //
//////////////////////////////////////////
class EnergyHolder : public Actor {
public:
    /* constructor/destructor */
    EnergyHolder(int imageID, int startX, int startY, int depth, StudentWorld* world, int energy) :
    Actor(imageID, startX, startY, depth, world), m_energy(energy) {}
    virtual ~EnergyHolder() = 0;           //implemented in Actor.cpp
    
    /* gain/lose energy */
    int getEnergy() const { return m_energy; }
    void gainEnergy(int amt);
    void loseEnergy(int amt);
    
private:
    int m_energy;
};

//////////////////////////////////////////
//                 EATER                //
//////////////////////////////////////////

class Eater : public EnergyHolder {
public:
    /* constructor/destructor */
    Eater(int imageID, int startX, int startY, int depth, StudentWorld* world, int energy) :
    EnergyHolder(imageID, startX, startY, depth, world, energy) {}
    
    virtual ~Eater() = 0;               //implemented in Actor.cpp
    
    /* Eating */
    virtual int Eat(int amt);           //returns units of Food actually eaten
    
private:
};

//////////////////////////////////////////
//                 INSECT               //
//////////////////////////////////////////

class Insect : public Eater {
public:
    /* constructor/destructor */
    Insect(int imageID, int startX, int startY, StudentWorld* world, int energy);
    virtual ~Insect() {}
    
    /* movement */
    void FaceRandDir();
    bool moveInDir(Direction dir);      //Returns false if it could not move
    
    /* death */
    void Die();                         // sets dead status and places 100 units of Food at current location
    
    /* biting */
    virtual void getBitten(int amt);
    
    /* poison */
    virtual void getPoisoned();
    
    /* sleep or stun */
    void increaseSleepOrStunTicks(int ticks) { m_SleepOrStunTicks += ticks; }
    virtual void getStunned();
    
    /* common actions of all insects */
    int initialInsectActions();         // returns 0 if Insect died, or if it is alive and sleeping/stunned in this tick
                                        // returns 2 otherwise
    
    /* Identifier functions */
    virtual bool amIEnemyOfAdultGrasshopper() const { return true; }
    
private:
    int m_SleepOrStunTicks;
    bool m_prevStunned;                 // remembers whether insect was stunned on current square
    
    /* sleep or stun */
    bool isSleepingOrStunned() const { return (m_SleepOrStunTicks > 0); }
    
    /* biting */
    virtual bool biteRandomEnemy() { return false; }
};

//////////////////////////////////////////
//             GRASSHOPPER              //
//////////////////////////////////////////

class Grasshopper : public Insect {
public:
    /* constructor/destructor */
    Grasshopper(int imageID, int startX, int startY, StudentWorld* world, int hitpoints) :
    Insect(imageID, startX, startY, world, hitpoints) { SetRandDist(); }
    virtual ~Grasshopper() {}
    
    /* do something */
    virtual void doSomething();
    
    /* Identifier functions */
    virtual bool amIEnemyOfAnt(int colonyNum) { return true; }
    
private:
    int m_desiredMoveDist;
    
    /* movement */
    void SetRandDist() { m_desiredMoveDist = randInt(2, 10); }
    int remainingMoveDist() const { return m_desiredMoveDist; }
    
    /* common actions of all derived classes */
    void postEatActions();                      //what all Grasshoppers do from when they start eating
    
    /* differing actions of derived classes */
    virtual int preEatActions() = 0;            //what specific derived classes do before eating
};

//////////////////////////////////////////
//          BABY GRASSHOPPER            //
//////////////////////////////////////////

class BabyGrasshopper : public Grasshopper {
public:
    /* constructor/destructor */
    BabyGrasshopper(int startX, int startY, StudentWorld* world) :
    Grasshopper(IID_BABY_GRASSHOPPER, startX, startY, world, 500) {}
    
    /* identifier functions */
    virtual bool canIBePoisoned() const { return true; }
    virtual bool canIBeStunned() const { return true; }
    
private:
    /* differing action of this derived class */
    virtual int preEatActions();            //returns 0 if BabyGrasshopper turned into AdultGrasshopper, otherwise returns 2
};

//////////////////////////////////////////
//          ADULT GRASSHOPPER           //
//////////////////////////////////////////

class AdultGrasshopper : public Grasshopper {
public:
    /* constructor/destructor */
    AdultGrasshopper(int startX, int startY, StudentWorld* world) :
    Grasshopper(IID_ADULT_GRASSHOPPER, startX, startY, world, 1600) {}
    
    /* biting */
    virtual void getBitten(int amt);

private:
    /* jumping */
    bool jumpToRandSquare();
    
    /* biting */
    virtual bool biteRandomEnemy();

    /* differing action of this derived class */
    virtual int preEatActions();        //returns 1 if AdultGrasshopper bit enemy or jumped, otherwise returns 2
};

//////////////////////////////////////////
//                 ANT                  //
//////////////////////////////////////////

class Ant : public Insect {   
public:
    /* constructor/destructor */
    Ant(int imageID, int startX, int startY, int colonyNum, StudentWorld* world, Compiler* compiler) :
    Insect(imageID, startX, startY, world, 1500), m_colonyNum( colonyNum ) , m_foodUnitsHeld(0), m_lastRandNum(0), m_instructionCounter(0) ,m_prevBitten(false), m_prevBlockedFromMoving(false), m_Compiler(compiler) {}
    
    virtual ~Ant() {}
    
    /* eat */
    virtual int Eat(int amt);
    
    /* colony number */
    int getColony() const { return m_colonyNum; }
    
    /* biting */
    virtual void getBitten(int amt);
    
    /* do something */
    virtual void doSomething();
    
   /* Identifier functions  */
    virtual bool amIEnemyOfAdultGrasshopper() const { return true; }
    virtual bool amIEnemyOfAnt(int colonyNum) { return getColony() != colonyNum; }
    virtual bool canIBePoisoned() const { return true; }
    virtual bool canIBeStunned() const { return true; }
    
private:
    int m_colonyNum;
    int m_foodUnitsHeld;
    int m_lastRandNum;
    int m_instructionCounter;
    bool m_prevBitten;
    bool m_prevBlockedFromMoving;
    Compiler* m_Compiler;
    
    /* biting */
    bool wasPreviouslyBitten() const { return m_prevBitten; }
    void setPreviouslyBitten() { m_prevBitten = true; }
    void setNotPreviouslyBitten() { m_prevBitten = false; }
    virtual bool biteRandomEnemy();
    
    /* blocked */
    bool wasPreviouslyBlocked() const { return m_prevBlockedFromMoving; }
    void setPreviouslyBlocked() { m_prevBlockedFromMoving = true; }
    void setNotPreviouslyBlocked() { m_prevBlockedFromMoving = false; }
    
    /* movement */
    void rotateClockwise();
    void rotateCounterClockwise();
    
    /* drop/pickup food */
    void dropFood();
    int pickUpFood();                           // returns units of Food picked up

    /* interpreter */
    int interpreter(Compiler::Command &cmd);    // returns 1 if simulation state has been changed, otherwise 0
    bool commandTriggered(Compiler::Command &cmd) const;
};

//////////////////////////////////////////
//                ANTHILL               //
//////////////////////////////////////////

class Anthill : public Eater {
public:
    
    /* constructor/destructor */
    Anthill(int startX, int startY, int colonyNum, StudentWorld* sw, Compiler* compiler) :
    Eater(IID_ANT_HILL, startX, startY, 2, sw, 8999), m_colonyNum(colonyNum), m_Compiler(compiler) {}
    
    ~Anthill() {}
    
    /* do something */
    virtual void doSomething();
    
private:
    int m_colonyNum;
    Compiler* m_Compiler;
    
    /* giving birth to ant */
    void createAnt();
};

//////////////////////////////////////////
//                  FOOD                //
//////////////////////////////////////////

class Food : public EnergyHolder {
public:
    
    /* constructor/destructor */
    Food(int x, int y, int units, StudentWorld* world) :
    EnergyHolder(IID_FOOD, x, y, 2, world, units) {}
    ~Food() {}
    
     /* Identifier functions */
    virtual bool canIBeEaten() const { return true; }
    
private:
};

//////////////////////////////////////////
//              PHEROMONE               //
//////////////////////////////////////////

class Pheromone : public EnergyHolder {
public:
    /* constructor/destructor */
    Pheromone(int imageID, int startX, int startY, int colonyNum, StudentWorld* world) :
    EnergyHolder(imageID, startX, startY, 2, world, 256), m_colonyNum(colonyNum) {}
    
    ~Pheromone() {}
    
    /* do something */
    virtual void doSomething() { loseEnergy(1); }
    
    /* colony number */
    int getColony() const { return m_colonyNum; }
    
    /* identifier functions */
    virtual bool amIPheromone() const { return true; }
    
private:
    int m_colonyNum;
};

//////////////////////////////////////////
//               IMPACTOR               //
//////////////////////////////////////////

class Impactor : public Actor {
public:
    /* constructor/destructor */
    Impactor(int imageID, int startX, int startY, StudentWorld* world) :
    Actor(imageID, startX, startY, 2, world) {}
    
    ~Impactor() {}
    
    /* do something */
    virtual void doSomething() { impactActors(); }
    
private:
    /* impact of specific derived classes */
    virtual void impactActors() = 0;
};

//////////////////////////////////////////
//                 POISON               //
//////////////////////////////////////////

class Poison : public Impactor {
public:
    /* constructor */
    Poison(int startX, int startY, StudentWorld* world) :
    Impactor(IID_POISON, startX, startY, world) {}
    
private:
    /* impact actors */
    virtual void impactActors();
};

//////////////////////////////////////////
//             POOL OF WATER            //
//////////////////////////////////////////

class PoolOfWater : public Impactor {
public:
    /* constructor */
    PoolOfWater(int startX, int startY, StudentWorld* world) :
    Impactor(IID_WATER_POOL, startX, startY, world) {}
    
private:
    /* impact actors */
    virtual void impactActors();
};

//////////////////////////////////////////
//                 PEBBLE               //
//////////////////////////////////////////

class Pebble : public Actor {
public:
    Pebble(int startX, int startY, StudentWorld* world) :
    Actor(IID_ROCK, startX, startY, 1, world) {}
    ~Pebble() {}
    
    /* Identifier functions */
    virtual bool amIWalkable() const { return false; }
private:
};

#endif // ACTOR_H_
