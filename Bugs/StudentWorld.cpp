#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include "Compiler.h"
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::~StudentWorld() {
    cleanUp();
}

bool StudentWorld::isSquareWalkable (int x, int y) const {
    
    if ( x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
        return false;
    
    for (list<Actor*>::const_iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end() ;it++) {
        if (!(*it)->amIWalkable())
            return false;
    }
    return true;
}

bool StudentWorld::checkEatable(int x, int y) const {
    
    if ( x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
        return false;
    
    for (list<Actor*>::const_iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end() ;it++) {
        if ((*it)->canIBeEaten() && !(*it)->isDead())       //cannot eat dead objects
            return true;
    }
    return false;
}

bool StudentWorld::PheromoneAheadOfAnt(const Ant *caller) const {
    int x=0;
    int y=0;
    
    switch (caller->getDirection()) {
        case GraphObject::right:
            x = caller->getX() + 1;
            y = caller->getY();
            break;
            
        case GraphObject::left:
            x = caller->getX() - 1;
            y = caller->getY();
            break;
            
        case GraphObject::up:
            x = caller->getX();
            y = caller->getY() + 1;
            break;
            
        case GraphObject::down:
            x = caller->getX();
            y = caller->getY() - 1;
            break;
            
        default:
            break;
    }
    
    for (list<Actor*>::const_iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++) {
        
        if ((*it)->amIPheromone() && !(*it)->isDead()) {
            Pheromone* php = dynamic_cast<Pheromone*>(*it);
            
            if (php->getColony() == caller->getColony())
                return true;
        }
    }
    
    return false;
}

bool StudentWorld::DangerAheadOfAnt(const Ant *caller) const {
    int x=0;
    int y=0;
    
    switch (caller->getDirection()) {
        case GraphObject::right:
            x = caller->getX() + 1;
            y = caller->getY();
            break;
            
        case GraphObject::left:
            x = caller->getX() - 1;
            y = caller->getY();
            break;
            
        case GraphObject::up:
            x = caller->getX();
            y = caller->getY() + 1;
            break;
            
        case GraphObject::down:
            x = caller->getX();
            y = caller->getY() - 1;
            break;
            
        default:
            break;
    }
    
    for (list<Actor*>::const_iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++) {
        if(!(*it)->isDead() && (*it)->amIEnemyOfAnt(caller->getColony()))
            return true;
    }
    
    return false;
}

int StudentWorld::countAntEnemies(const Ant *caller) const {
    
    int enemyCount = 0;
    int x = caller->getX();
    int y = caller->getY();
    for (list<Actor*>::const_iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++) {
     
        if (!(*it)->isDead() && (*it)->amIEnemyOfAnt(caller->getColony()))
            enemyCount++;
    }
    return enemyCount;
}

void StudentWorld::addOrCreateFood(int x, int y, int units) {
    
    if (units == 0)
        return;
    
    Food* fp;
    
    for (list<Actor*>::iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end() ;it++) {
        if ((*it)->canIBeEaten() && !(*it)->isDead()) {     //Don't want to add units to dead Food object
            fp = dynamic_cast<Food*>(*it);
            fp->gainEnergy(units);
            return;
        }
    }
    
    fp = new Food(x, y, units, this);
    m_ActorLists[x][y].push_back(fp);
}

void StudentWorld::addOrCreatePheromone(int x, int y, int colonyNum) {
    
    Pheromone* php;
    
    for (list<Actor*>::iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++) {
        
        if ((*it)->amIPheromone() && !(*it)->isDead()) {
            php = dynamic_cast<Pheromone*>(*it);
            
            if (php->getColony() == colonyNum) {
                
                if (php->getEnergy() <= 512)
                    php->gainEnergy(256);
                else
                    php->gainEnergy(768-php->getEnergy());
                return;
            }
        }
    }
    
    if (colonyNum == 0)
        m_ActorLists[x][y].push_back(new Pheromone(IID_PHEROMONE_TYPE0, x, y, 0, this));
    if (colonyNum == 1)
        m_ActorLists[x][y].push_back(new Pheromone(IID_PHEROMONE_TYPE1, x, y, 1, this));
    if (colonyNum == 2)
        m_ActorLists[x][y].push_back(new Pheromone(IID_PHEROMONE_TYPE2, x, y, 2, this));
    if (colonyNum == 3)
        m_ActorLists[x][y].push_back(new Pheromone(IID_PHEROMONE_TYPE3, x, y, 3, this));
}

void StudentWorld::createAdultGrasshopper(int x, int y) { 
    AdultGrasshopper* agp = new AdultGrasshopper(x, y, this);
    m_ActorLists[x][y].push_back(agp);
}

void StudentWorld::addAnt(Ant* ant) {
    m_ActorLists[ant->getX()][ant->getY()].push_back(ant);
    
    for (int i = 0; i < MAX_ANT_COLONIES; i++) {
        if (i == ant->getColony())
            m_AntCounts[i]++;
    }
    
    updateWinner();
}

int StudentWorld::eatOrPickUpFood(int x, int y, int amt) {
    
    if (amt==0)
        return 0;
    
    Food* fp;
    for (list<Actor*>::iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end() ;it++) {
        if ((*it)->canIBeEaten() && !(*it)->isDead()) {     //Don't want dead Food object
            fp = dynamic_cast<Food*>(*it);
            if (fp -> getEnergy() >= amt) {
                fp->loseEnergy(amt);
                return amt;
            }
            else {
                int res = fp-> getEnergy();
                fp->loseEnergy(res);
                return res;
            }
        }
    }
    return 0;
}

bool StudentWorld::biteRandAdultGrasshopperEnemy(AdultGrasshopper* caller) {
    
    int enemyCount = countAdultGrasshopperEnemies(caller);
    int x = caller->getX();
    int y = caller->getY();
    
    if (enemyCount == 0)
        return false;
            
    int rand = randInt(1, enemyCount);
            
    int i = 0;
    list<Actor*>::iterator it;
    for (it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++ ) {
                
        if ((*it)->amIEnemyOfAdultGrasshopper()) {
            if (*it != caller && !(*it)->isDead())  //Doesn't want to attack itself or dead insects!
                i++;
        }
        if (i == rand)
            break;
    }
            
    Insect* ip = dynamic_cast<Insect*>(*it);
    ip->getBitten(50);
    return true;
}

bool StudentWorld::biteRandAntEnemy(Ant *caller) {
    
    int enemyCount = countAntEnemies(caller);
    int x = caller->getX();
    int y = caller->getY();
    
    if (enemyCount == 0)
        return false;
    
    int rand = randInt(1, enemyCount);
    
    int i = 0;
    list<Actor*>::iterator it;
    for (it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++) {
        
        if(!(*it)->isDead() && (*it)->amIEnemyOfAnt(caller->getColony()))
            i++;
        
        if (i == rand)
            break;
    }
    
    Insect* ip = dynamic_cast<Insect*>(*it);
    ip->getBitten(15);
    return true;
}

void StudentWorld::poisonActorsAt(int x, int y) {
    for (list<Actor*>::iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++)
        (*it)->getPoisoned();
}

void StudentWorld::stunActorsAt(int x, int y) {
    for (list<Actor*>::iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++)
        (*it)->getStunned();
}

/* PRIVATE MEMBER FUNCTIONS */

int StudentWorld::initCompilers() {
    vector<string> fileNames = getFilenamesOfAntPrograms();
    
    string err;
    for (int i = 0; i<fileNames.size(); i++) {
        m_compilers.push_back(new Compiler);
        if (!m_compilers[i]->compile(fileNames[i], err)) {
            setError(fileNames[i] + " " + err);
            return -999;
        }
    }
    return 0;
}

void StudentWorld::makeActorsDoSomething() {

    for (int i = 0; i < VIEW_WIDTH; i++) {     //Enumerate all Actors
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            for (list<Actor*>::iterator it=m_ActorLists[i][j].begin(); it!=m_ActorLists[i][j].end();) {
                    
                if (!(*it)->hasDoneSomething())
                    (*it)->doSomething();
                else {
                    it++;
                    continue;
                }
                    
                (*it)->setDoneSomething();
                    
                if(i != (*it)->getX() || j != (*it)-> getY()) { //if Actor moved
                    m_ActorLists[(*it)->getX()][(*it)->getY()].push_back(*it); //Update location
                    it = m_ActorLists[i][j].erase(it);
                }
                else
                    it++;
            }
        }
    }
    
}

void StudentWorld::removeDeadActors() {
    for (int i = 0; i < VIEW_WIDTH; i++) {
        for (int j = 0; j < VIEW_HEIGHT; j++) {
            for (list<Actor*>::iterator it=m_ActorLists[i][j].begin(); it!=m_ActorLists[i][j].end();)
                
                if ((*it)->isDead()) {
                    delete (*it);
                    it = m_ActorLists[i][j].erase(it);
                }
                else {
                    (*it)->setNotDoneSomething();   //prepare for next tick
                    it++;
                }
        }
    }
}

void StudentWorld::deleteCompilers() {
    for (vector<Compiler*>::iterator it = m_compilers.begin(); it != m_compilers.end();) {
        delete *it;
        it = m_compilers.erase(it);
    }
}

int StudentWorld::countAdultGrasshopperEnemies(const AdultGrasshopper* caller) const {
    
    int enemyCount = 0;
    int x = caller->getX();
    int y = caller->getY();
    for (list<Actor*>::const_iterator it = m_ActorLists[x][y].begin(); it != m_ActorLists[x][y].end(); it++) {
        
        if ((*it)->amIEnemyOfAdultGrasshopper()) {
            if (*it != caller && !(*it)->isDead())          //Doesn't want to attack itself or dead insects!
                enemyCount++;
        }
    }
    return enemyCount;
}

void StudentWorld::updateWinner() {
    
    for (int i = 0; i < m_compilers.size(); i++) {
        for (int j = 0; j < m_compilers.size(); j++) {
            
            if (m_AntCounts[i] <= m_AntCounts[j] && i!=j)
                break;
            
            if (j == m_compilers.size() - 1 && m_AntCounts[i] > 5) {
                m_currentWinner = i;
                return;
            }
        }
    }
    
}

void StudentWorld::updateDisplayText() {   
    
    ostringstream oss;
    oss << "Ticks:" << setw(5) << 2000-m_tickCount << " - ";    //We count down, as in provided simulation
    
    oss.fill('0');
    for (int i = 0; i < m_compilers.size(); i++) {
        
        if (m_currentWinner == i)
            oss << m_compilers[i]->getColonyName() << "*: " << setw(2) << m_AntCounts[i] << "  ";
        else
            oss << m_compilers[i]->getColonyName() << ": " << setw(2) << m_AntCounts[i] << "  ";
    }
    setGameStatText(oss.str());
}