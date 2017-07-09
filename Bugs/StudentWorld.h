#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <list>
#include <map>
using namespace std;

class Compiler;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}
    
    ~StudentWorld();

	virtual int init()
	{
        m_tickCount = 0;
        for (int i = 0; i < MAX_ANT_COLONIES; i++)
            m_AntCounts[i] = 0;
        m_currentWinner = -999;
        
        if (initCompilers() == -999)
            return GWSTATUS_LEVEL_ERROR;
        
        Field f;
        string fieldFile = getFieldFilename();
        //string fieldFile = "/Users/akshaysmit/Desktop/simplefield.txt";
        
        string error;
        if (f.loadField(fieldFile, error) != Field::LoadResult::load_success) { //Something bad happened
            setError(fieldFile + " " + error);
            return false;
        }
        
        /* Begin creation of game world */
        for (int x = 0; x < VIEW_WIDTH; x++) {
            for (int y = 0; y < VIEW_HEIGHT; y++) {
                Field::FieldItem curr_item = f.getContentsOf(x, y);
                
                switch (curr_item) {
                    case Field::anthill0:
                        m_colonyX[0] = x;
                        m_colonyY[0] = y;
                        m_ActorLists[x][y].push_back(new Anthill(x, y, 0, this, m_compilers[0]));
                        break;
                        
                    case Field::anthill1:
                        if (m_compilers.size() <= 1)
                            break;
                        m_colonyX[1] = x;
                        m_colonyY[1] = y;
                        m_ActorLists[x][y].push_back(new Anthill(x, y, 1, this, m_compilers[1]));
                        break;
                        
                    case Field::anthill2:
                        if (m_compilers.size() <= 2)
                            break;
                        m_colonyX[2] = x;
                        m_colonyY[2] = y;
                        m_ActorLists[x][y].push_back(new Anthill(x, y, 2, this, m_compilers[2]));
                        break;
                        
                    case Field::anthill3:
                        if (m_compilers.size() <= 3)
                            break;
                        m_colonyX[3] = x;
                        m_colonyY[3] = y;
                        m_ActorLists[x][y].push_back(new Anthill(x, y, 3, this, m_compilers[3]));
                        break;
                        
                    case Field::food:
                        addOrCreateFood(x, y, 6000);
                        break;
                        
                    case Field::grasshopper: {
                        BabyGrasshopper* babyp = new BabyGrasshopper(x, y, this);
                        m_ActorLists[x][y].push_back(babyp);
                        break;
                    }
                        
                    case Field::water:
                        m_ActorLists[x][y].push_back(new PoolOfWater(x, y, this));
                        break;
                        
                    case Field::rock: {
                        Pebble* pebp = new Pebble(x, y, this);
                        m_ActorLists[x][y].push_back(pebp);
                        break;
                    }
                        
                    case Field::poison:
                        m_ActorLists[x][y].push_back(new Poison(x, y, this));
                        break;
                        
                    case Field::empty:
                        break;
                        
                }
            }
        }
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
        m_tickCount++;
        makeActorsDoSomething();
        removeDeadActors();
        updateDisplayText();
        
        if (m_tickCount >= 2000) {
            
            if (m_currentWinner == -999)
                return GWSTATUS_NO_WINNER;
            
            for (int i = 0; i < m_compilers.size(); i++) {
                if (m_currentWinner == i) {
                    setWinner(m_compilers[i]->getColonyName());
                    return GWSTATUS_PLAYER_WON;
                }
            }
        }
        
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter.
		  // Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual void cleanUp()
	{
        for (int i = 0; i < VIEW_WIDTH; i++) {
            for (int j = 0; j < VIEW_HEIGHT; j++) {
                for (list<Actor*>::iterator it = m_ActorLists[i][j].begin(); it != m_ActorLists[i][j].end();) {
                    delete *it;
                    it = m_ActorLists[i][j].erase(it);
                }
            }
        }
        deleteCompilers();
	}
    
    /* check for Actor-derived objects */
    bool isSquareWalkable(int x, int y) const;
    bool checkEatable(int x, int y) const;
    bool PheromoneAheadOfAnt(const Ant* caller) const;
    bool DangerAheadOfAnt(const Ant* caller) const;
    int countAntEnemies(const Ant* caller) const;
    int getColonyX(int colonyNum) const { return m_colonyX[colonyNum]; }
    int getColonyY(int colonyNum) const { return m_colonyY[colonyNum]; }
    
    
    /* modify Actor-derived objects */
    bool biteRandAdultGrasshopperEnemy(AdultGrasshopper* caller);
    bool biteRandAntEnemy(Ant* caller);
    int eatOrPickUpFood(int x, int y, int amt);             // returns units of Food eaten/picked up from square
    void poisonActorsAt(int x, int y);
    void stunActorsAt(int x, int y);
    
    /* insert Actor-derived objects */
    void addOrCreateFood(int x, int y, int units);
    void addOrCreatePheromone(int x, int y, int colonyNum); //if Pheromone present, increases its strength by 256
                                                            //up to a maximum of 768
    void createAdultGrasshopper(int x, int y);
    void addAnt(Ant* ant);                                  // add an existing Ant object to the simulation
    
private:
    list<Actor*> m_ActorLists[VIEW_WIDTH][VIEW_HEIGHT];
    vector<Compiler*> m_compilers;
    int m_colonyX[MAX_ANT_COLONIES];                      // X coordinates of Anthills
    int m_colonyY[MAX_ANT_COLONIES];                      // Y coordinates of Anthills
    int m_AntCounts[MAX_ANT_COLONIES];
    int m_tickCount;
    int m_currentWinner;                                //returns colony number of winner, -999 if no winner

    /* private member functions */
    int initCompilers();                                //returns -999 upon failure
    void makeActorsDoSomething();
    void removeDeadActors();
    void deleteCompilers();
    void updateWinner();
    void updateDisplayText();
    
        //Does not consider the caller to be an enemy of itself:
    int countAdultGrasshopperEnemies(const AdultGrasshopper* caller) const;
};

#endif // STUDENTWORLD_H_
