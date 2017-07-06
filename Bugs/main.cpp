#include "GameController.h"
#include <iostream>
#include <fstream>
#include <list>
#include <string>
using namespace std;

  // If your program is having trouble finding the Assets directory,
  // replace the string literal with a full path name to the directory,
  // e.g., "Z:/CS32/Bugs/Assets" or "/Users/fred/cs32/Bugs/Assets"

const string assetDirectory = "/Users/akshaysmit/Desktop/CS32/Proj3/Bugs/DerivedData/Bugs/Build/Products/Debug/Assets";

class GameWorld;

GameWorld* createStudentWorld(string assetDir = "");

int main(int argc, char* argv[])
{
    {
		string path = assetDirectory;
		if (!path.empty())
			path += '/';
		const string someAsset = "rock1.tga";
		ifstream ifs(path + someAsset);
		if (!ifs)
		{
			cout << "Cannot find " << someAsset << " in ";
			cout << (assetDirectory.empty() ? "current directory" : assetDirectory) << endl;
			return 1;
		}
	}

	GameWorld* gw = createStudentWorld(assetDirectory);
	Game().run(argc, argv, gw, "Bugs");
}

/*
#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <cassert>
using namespace std;

void testBabyGrasshopper() {
    
    StudentWorld sw("dummystr");
    assert(sw.isSquareWalkable(2, 3));
    
    for (int i = 0; i < 64; i++) {
        sw.tempInsertPebble(i, 0);
        sw.tempInsertPebble(0, i);
    }
    
    for (int i = 0; i < 64; i++) {
        sw.tempInsertPebble(i, 63);
        sw.tempInsertPebble(63, i);
    }
    
    Pebble rocky(6, 9, &sw);
    rocky.doSomething();
    assert(rocky.getID() == IID_ROCK);
    assert(rocky.getX() == 6);
    assert(rocky.getY() == 9);
    assert(rocky.getDirection() == GraphObject::right);
    
    sw.addOrCreateFood(50, 50, 400);
    sw.addOrCreateFood(50, 50, 400);
    //Check Food object in sw here
    assert(sw.eatOrPickUpFood(4, 30, 200) == 0);
    assert(sw.eatOrPickUpFood(49, 49, 200) == 0);
  
    BabyGrasshopper hopper(50, 50, &sw);
    assert(hopper.getID() == IID_BABY_GRASSHOPPER);
    assert(hopper.getX() == 50);
    assert(hopper.getY() == 50);
    assert(hopper.getWorld() == &sw);
    
    if (hopper.getDirection() == GraphObject::right)
        cout << "right" << endl;
    if (hopper.getDirection() == GraphObject::left)
        cout << "left" << endl;
    if (hopper.getDirection() == GraphObject::up)
        cout << "up" << endl;
    if (hopper.getDirection() == GraphObject::down)
        cout << "down" << endl;

    cout << "Initial chosen distance is " << hopper.remainingMoveDist() << endl;

    assert(hopper.getEnergy() == 500);
    assert(!hopper.isDead());
    assert(!hopper.isSleepingOrStunned());
    
    hopper.doSomething();
    if (hopper.getEnergy() == 499)
        assert(hopper.isSleepingOrStunned());
    if (hopper.getEnergy() == 699)
        cout << "hopper ate food" << endl;
    
    if (hopper.isSleepingOrStunned())
        cout << "sleeping" << endl;
    
    hopper.doSomething();
    assert(hopper.getEnergy() == 698);
    assert(hopper.isSleepingOrStunned());
    int x = hopper.getX();
    int y = hopper.getY();

    hopper.doSomething();
    assert(!hopper.isSleepingOrStunned());
    assert(hopper.getHitpoints() == 697);
    assert(x == hopper.getX());
    assert(y == hopper.getY());
    
    hopper.doSomething();
    assert(hopper.isSleepingOrStunned());
    
    for (int i = 0; i < 1000; i++) {
        hopper.doSomething();
        if (hopper.isDead()) {
            x = hopper.getX();
            y = hopper.getY();
            cout << "Died at " <<x<< " "<< y << endl;
            break;
        } 
    }
    
    BabyGrasshopper hopper2(4, 4, &sw);
    assert(!hopper2.isDead());
} */
/*
#include <list>
#include <iostream>
using namespace std;

int main() {
    
    list<int> li;
    li.push_back(5);
    li.push_back(4);
    li.push_back(10);
    li.push_back(5);
    li.push_back(11);
    li.push_back(12);
    li.push_back(5);
    li.push_back(13);
    
    int cnt;
    for (list<int>::iterator it = li.begin(); it!=li.end(); it++) {
        if (*it != 5)
            cnt++;
    }
    
    int rand = 5; //index
    cout << rand << endl;
    list<int>::iterator it;
    int i = 0;
    
    for (it = li.begin(); it != li.end(); it++) {
        
        if (*it!=5)
            ++i;
        
        if (i == rand) {
            break;
        }
    }
    cout << *it << endl;
}

#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
    Compiler c;
    StudentWorld sw("sss");
    sw.addOrCreateFood(50, 50, 1000);
    
    Ant* annt = new Ant(IID_ANT_TYPE0 ,50, 50, 0, &sw, &c);
    assert(!annt->isDead());
    assert(annt->Eat(100) == 0);
    assert(annt->getEnergy() == 1500);
    annt->moveTo(4, 4);
    annt->dropFood();
    assert(annt->getX() == 4 && annt->getY() == 4);
    assert(annt->pickUpFood() == 0);
    assert(annt->Eat(100) == 0);
    
    annt->moveTo(50, 50);
    assert(annt->getX() == 50 && annt->getY() == 50);
    assert(annt->pickUpFood() == 400);
    assert(annt->getEnergy() == 1500);
    assert(annt->Eat(100) == 100);
    assert(annt->getEnergy() == 1600);
    assert(annt->Eat(100) == 100);
    assert(annt->getEnergy() == 1700);
    assert(annt->Eat(100) == 100);
    //Check units of food held
    
    assert(annt->getEnergy() == 1800);
    assert(annt->Eat(100) == 100);
    assert(annt->getEnergy() == 1900);
    //Check units of food held
    
    assert(annt->Eat(100) == 0);
    assert(annt->getEnergy() == 1900);
    
    sw.addOrCreateFood(20, 20, 80);
    annt->moveTo(20, 20);
    
    assert(annt->Eat(100) == 0);
    assert(annt->pickUpFood() == 80);
    assert(annt->getEnergy() == 1900);
    assert(annt->Eat(100) == 80 && annt->getEnergy() == 1980);
    assert(annt->Eat(100) == 0 && annt->getEnergy() == 1980);
    //Check there is dead Food object at (20, 20)
    
    sw.addOrCreateFood(20, 20, 1900);
    //Check there is one dead and one not-dead Food object at (20, 20)
    
    assert(annt->Eat(100) == 0 && annt->getEnergy() == 1980);
    assert(annt->pickUpFood() == 400);
    assert(annt->pickUpFood() == 400);
    assert(annt->pickUpFood() == 400);
    assert(annt->pickUpFood() == 400);
    assert(annt->pickUpFood() == 200);
    assert(annt->pickUpFood() == 0 && annt->getEnergy() == 1980);
    
    for (int i = 1; i < 19; i++) {
        assert(annt->Eat(100) == 100);
        assert(annt->getEnergy() == 1980+i*100);
    }
    
    assert(annt->Eat(100) == 0 && annt->getEnergy() == 3780);
    
    assert(annt->getX() == 20 && annt->getY());
    sw.tempAddActor(annt);
    
    cout << "Passed all tests" << endl;
}

#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include "GameConstants.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
    Compiler c;
    string err;
    assert(c.compile("/Users/akshaysmit/Downloads/Bugs/USCAnt.bug", err));
    
    StudentWorld sw("sw");
    Ant* ap = new Ant(IID_ANT_TYPE0, 50, 50, 0, &sw, &c);
    sw.tempAddActor(ap);
    
    if (ap->getDirection() == GraphObject::right)
        cout << "right" << endl;
    if (ap->getDirection() == GraphObject::left)
        cout << "left" << endl;
    if (ap->getDirection() == GraphObject::up)
        cout << "up" << endl;
    if (ap->getDirection() == GraphObject::down)
        cout << "down" << endl;
    
    ap->doSomething();
    
    if (ap->getDirection() == GraphObject::right)
        cout << "right" << endl;
    if (ap->getDirection() == GraphObject::left)
        cout << "left" << endl;
    if (ap->getDirection() == GraphObject::up)
        cout << "up" << endl;
    if (ap->getDirection() == GraphObject::down)
        cout << "down" << endl;
    
    assert(ap->getX() == 50 && ap->getY() == 50);
    
    ap->doSomething();
    
    if (ap->getDirection() == GraphObject::right)
        assert(ap->getX() == 51 && ap->getY() == 50);
    else if (ap->getDirection() == GraphObject::left)
        assert(ap->getX() == 49 && ap->getY() == 50);
    else if (ap->getDirection() == GraphObject::up)
        assert(ap->getX() == 50 && ap->getY() == 51);
    else if (ap->getDirection() == GraphObject::down)
        assert(ap->getX() == 50 && ap->getY() == 49);
    else
        cout << "error" << endl;
    
    Food fd(3, 3, 1000, &sw);
    assert(fd.getDirection() == GraphObject::right);
    assert(!fd.isDead());
    assert(fd.getEnergy() == 1000);
    assert(fd.getX() == 3 && fd.getY() == 3);
    
    fd.loseEnergy(100);
    assert(!fd.isDead() && fd.getEnergy() == 900);
    
    fd.gainEnergy(50);
    assert(fd.getEnergy() == 950);
    
    fd.loseEnergy(1000);
    assert(fd.isDead());
    
    Food* fp = new Food(10, 10, 400, &sw);
    sw.tempAddActor(fp);
    sw.eatOrPickUpFood(10, 10, 200);
    assert(!fp->isDead() && fp->getEnergy() == 200);
    
    sw.addOrCreateFood(10, 10, 100);
    assert(fp->getEnergy() == 300);
    
    assert(sw.eatOrPickUpFood(10, 10, 400) == 300);
    assert(fp->getEnergy() == 0);
    assert(fp->isDead());
    
    Anthill ah(60, 60, 1, &sw, &c);
    assert(ah.getDirection() == GraphObject::right);
    assert(ah.getX() == 60 && ah.getY() == 60);
    assert(!ah.isDead());
    assert(ah.getEnergy() == 8999);
    assert(ah.Eat(10000) == 0);
    assert(ah.getEnergy() == 8999);
    
    ah.doSomething();
    assert(ah.getEnergy() == 7498);
    //Check ant object at (60, 60) here
    
    Food* antfood = new Food(60, 60, 1000, &sw);
    sw.tempAddActor(antfood);
    
    ah.doSomething();
    assert(antfood->isDead());
    assert(ah.getEnergy() == 8497);
    
    ah.doSomething();   // -1500 energy
    ah.doSomething();   // -1500 energy
    ah.doSomething();   // -1500 energy
    ah.doSomething();   // -1500 energy
    ah.doSomething();   // -1500 energy
    assert(ah.getEnergy() == 992);
    
    ah.doSomething();
    assert(ah.getEnergy() == 991);
    //Check exactly 6 ants at (60, 60)
    
    Ant* ap2 = new Ant(IID_ANT_TYPE3, 60, 60, 3, &sw, &c);
    sw.tempAddActor(ap2);
    assert(sw.countAntEnemies(ap2) == 6);
    
    Ant* ap3 =new Ant(IID_ANT_TYPE3, 60, 60, 3, &sw, &c);
    sw.tempAddActor(ap3);
    assert(sw.countAntEnemies(ap2) == 6);
    assert(sw.countAntEnemies(ap3) == 6);
    
    ap3->getBitten(100);
    assert(!ap3->isDead() && ap3->getEnergy() == 1400);
    
    cout << "Passed all tests" << endl;
}

#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include "GameConstants.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
    // testing StudentWorld functions
    
    Compiler c;
    StudentWorld sw("");
    Food* fp = new Food(50, 50, 1000, &sw);
    sw.tempAddActor(fp);
    assert(!sw.checkEatable(4, 4));
    assert(!sw.checkEatable(50, 49));
    assert(sw.checkEatable(50, 50));
    
    fp->loseEnergy(1100);
    assert(!sw.checkEatable(50, 50));
    
    fp = new Food(50, 50, 300, &sw);
    sw.tempAddActor(fp);
    assert(sw.checkEatable(50, 50));
    
    fp->loseEnergy(400);
    assert(!sw.checkEatable(50, 50));
    
    Ant* ap = new Ant(IID_ANT_TYPE1, 49, 50, 1, &sw, &c);
    ap->setDirection(GraphObject::right);
    sw.tempAddActor(ap);
    assert(!sw.DangerAheadOfAnt(ap));
    
    sw.tempAddActor(new Pebble(49, 49, &sw));
    ap->setDirection(GraphObject::down);
    assert(!sw.DangerAheadOfAnt(ap));
    
    ap->setDirection(GraphObject::up);
    assert(!sw.DangerAheadOfAnt(ap));
    
    sw.tempAddActor(new AdultGrasshopper(49, 51, &sw));
    assert(sw.DangerAheadOfAnt(ap));
    
    ap->setDirection(GraphObject::down);
    assert(!sw.DangerAheadOfAnt(ap));
    
    ap->setDirection(GraphObject::left);
    assert(!sw.DangerAheadOfAnt(ap));
    
    ap->setDirection(GraphObject::right);
    assert(!sw.DangerAheadOfAnt(ap));
    
    
    ap->setDirection(GraphObject::left);
    sw.tempAddActor(new Ant(IID_ANT_TYPE0, 48, 50, 0, &sw, &c));
    assert(sw.DangerAheadOfAnt(ap));
    
    cout << "Passed all tests" << endl;
}

#include "Actor.h"
#include "StudentWorld.h"
#include "Compiler.h"
#include "GameConstants.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
    Compiler c;
    string err;
    assert(c.compile("/Users/akshaysmit/Downloads/Bugs/USCAnt.bug", err));
    
    StudentWorld sw("sw");
    Ant* ap = new Ant(IID_ANT_TYPE0, 50, 50, 0, &sw, &c);
    Food    *f1, *f2, *f3, *f4;
    sw.tempAddActor(f1 = new Food(50, 49, 400, &sw));       // down
    sw.tempAddActor(f2 = new Food(49, 50, 400, &sw));       // left
    sw.tempAddActor(f3 = new Food(51, 50, 400, &sw));       // right
    sw.tempAddActor(f4 = new Food(50, 51, 400, &sw));       // up
    
    ap->doSomething();      //will face random direction
    
    if (ap->getDirection() == GraphObject::right)
        cout << "right" << endl;
    if (ap->getDirection() == GraphObject::left)
        cout << "left" << endl;
    if (ap->getDirection() == GraphObject::up)
        cout << "up" << endl;
    if (ap->getDirection() == GraphObject::down)
        cout << "down" << endl;
    
    GraphObject::Direction dir = ap->getDirection();
    
    ap->doSomething();      //will move forward
    ap->doSomething();      //will pickup food
    
    if (dir == GraphObject::down)
        assert(f1->isDead());
    if (dir == GraphObject::left)
        assert(f2->isDead());
    if (dir == GraphObject::right)
        assert(f3->isDead());
    if (dir == GraphObject::up)
        assert(f4->isDead());
    
    ap->getBitten(1480);
    
    Compiler::Command cmd;
    cmd.opcode = Compiler::eatFood;
    ap->interpreter(cmd);
    assert(ap->getEnergy() == 117);
    
    cmd.opcode = Compiler::if_command;
    cmd.operand1 = to_string(Compiler::i_am_hungry);
    assert(!ap->commandTriggered(cmd));
    
    cmd.operand1 = to_string(Compiler::i_am_carrying_food);
    assert(ap->commandTriggered(cmd));
    
    cmd.operand1 = to_string(Compiler::i_was_bit);
    assert(ap->commandTriggered(cmd));
    
    cmd.operand1 = to_string(Compiler::i_am_standing_with_an_enemy);
    assert(!ap->commandTriggered(cmd));
    
    sw.tempAddActor(new Ant(IID_ANT_TYPE0, ap->getX(), ap->getY(), 0, &sw, &c));
    assert(!ap->commandTriggered(cmd));
    
    sw.tempAddActor(new BabyGrasshopper(ap->getX(), ap->getY(), &sw));
    sw.tempAddActor(new AdultGrasshopper(ap->getX(), ap->getY(), &sw));
    sw.tempAddActor(new Ant(IID_ANT_TYPE0, ap->getX(), ap->getY(), 0, &sw, &c));
    sw.tempAddActor(new Ant(IID_ANT_TYPE1, ap->getX(), ap->getY(), 1, &sw, &c));
    assert(ap->commandTriggered(cmd));
    assert(sw.countAntEnemies(ap) == 3);
    
    ap->setDirection(GraphObject::up);
    cmd.operand1 = to_string(Compiler::i_smell_danger_in_front_of_me);
    sw.tempAddActor(new Ant(IID_ANT_TYPE0, ap->getX(), ap->getY()+1, 0, &sw, &c));
    assert(!ap->commandTriggered(cmd));

    sw.tempAddActor(new Ant(IID_ANT_TYPE1, ap->getX(), ap->getY()+1, 1, &sw, &c));
    assert(ap->commandTriggered(cmd));
    
    delete ap;
    sw.cleanUp();
    
    // Pheromones
    Pheromone pher(IID_PHEROMONE_TYPE0, 5, 5, 0, &sw);
    assert(pher.getEnergy() == 256);
    pher.doSomething();
    assert(pher.getEnergy() == 255 && !pher.isDead());
    for (int i = 0; i < 255; i++)
        pher.doSomething();
    assert(pher.getEnergy() == 0 && pher.isDead());
    
    sw.addOrCreatePheromone(10, 10, 1);
    sw.addOrCreatePheromone(10, 10, 1);
    //Check Pheromone at (10, 10) with strength 512
    
    sw.addOrCreatePheromone(10, 10, 1);
    sw.addOrCreatePheromone(10, 10, 1);
    sw.addOrCreatePheromone(10, 10, 1);
    //Check Pheromone at(10, 10) with strength 768
    
    ap = new Ant(IID_ANT_TYPE1, 9, 10, 1, &sw, &c);
    ap->setDirection(GraphObject::right);
    
    cmd.opcode = Compiler::if_command;
    cmd.operand1 = to_string(Compiler::i_smell_pheromone_in_front_of_me);
    assert(ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::left);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::down);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::up);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::right);
    assert(ap->commandTriggered(cmd));
    
    sw.addOrCreatePheromone(10, 10, 3); //Check two Pheromone objects at (10, 10)
    sw.addOrCreatePheromone(10, 10, 3);
    
    ap->setDirection(GraphObject::left);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::down);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::up);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::right);
    assert(ap->commandTriggered(cmd));
    
    delete ap;
    ap = new Ant(IID_ANT_TYPE3, 10, 9, 3, &sw, &c);
    ap->setDirection(GraphObject::up);
    
    assert(ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::left);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::down);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::right);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::up);
    assert(ap->commandTriggered(cmd));

    ap->moveTo(40, 40);
    cmd.opcode = Compiler::emitPheromone;
    assert(ap->interpreter(cmd) == 1);
    assert(ap->interpreter(cmd) == 1);
    
    ap->moveTo(40, 41);
    cmd.opcode = Compiler::if_command;
    cmd.operand1 = to_string(Compiler::i_smell_pheromone_in_front_of_me);
    
    ap->setDirection(GraphObject::down);
    assert(ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::up);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::left);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::right);
    assert(!ap->commandTriggered(cmd));
    
    ap->setDirection(GraphObject::down);
    assert(ap->commandTriggered(cmd));

    cmd.opcode = Compiler::emitPheromone;
    
    delete ap;
    ap = new Ant(IID_ANT_TYPE0, 40, 40, 0, &sw, &c);
    assert(ap->interpreter(cmd) == 1);
    
    delete ap;
    ap = new Ant(IID_ANT_TYPE1, 40, 40, 1, &sw, &c);
    assert(ap->interpreter(cmd) == 1);
    
    delete ap;
    ap = new Ant(IID_ANT_TYPE2, 40, 40, 2, &sw, &c);
    assert(ap->interpreter(cmd) == 1);
    assert(ap->interpreter(cmd) == 1);
    
    //Check 4 pheromones at 40, 40
    delete ap;
    cout << "Passed all tests" << endl;
}
*/
/*
#include "StudentWorld.h"
#include "Actor.h"
#include <iostream>
#include <cassert>
using namespace std;

int main() {
    StudentWorld sw("sw");
    Compiler c;
    
    AdultGrasshopper agh(5, 5, &sw);
    assert(!agh.canIBeStunned() && !agh.canIBePoisoned());
    agh.getPoisoned();
    assert(agh.getEnergy() == 1600);
    
    agh.getStunned();
    //Check it is not sleeping using breakpoint
    
    BabyGrasshopper bgh(5, 5, &sw);
    assert(bgh.canIBeStunned() && bgh.canIBePoisoned());
    bgh.getPoisoned();
    assert(bgh.getEnergy() == 350);
    
    bgh.getStunned();
    //Check it is sleeping using breakpoint
    
    bgh.getStunned();
    //Check sleeping ticks are still 2 using breakpoint
    
    bgh.moveInDir(GraphObject::right);
    //Check m_PrevStunned is false
    
    PoolOfWater* pool = new PoolOfWater(50, 50, &sw);
    assert(pool->getDirection() == GraphObject::right && !pool->isDead());
    assert(pool->getX() == 50 && pool->getY() == 50);
    sw.tempAddActor(pool);
    BabyGrasshopper* bgp;
    AdultGrasshopper* agp;
    Ant* ap;
    sw.tempAddActor(bgp = new BabyGrasshopper(50, 50, &sw));
    sw.tempAddActor(agp = new AdultGrasshopper(50, 50, &sw));
    sw.tempAddActor(ap = new Ant(IID_ANT_TYPE2, 50, 50, 2, &sw, &c));
    
    pool->doSomething();
    //Check AdultGrasshopper is not stunned, BabyGrasshopper and Ant are stunned, using breakpoint
    
    pool->doSomething();
    //Check AdultGrasshopper is not stunned, BabyGrasshopper and Ant are stunned for 2 ticks, using breakpoint
    
    Anthill* anth = new Anthill(32, 32, 3, &sw, &c);
    sw.tempAddActor(anth);
    assert(anth->getDirection() == GraphObject::right && anth->getEnergy() == 8999);
    anth->createAnt();
    anth->createAnt();
    anth->createAnt();
    anth->createAnt();
    anth->createAnt();
    anth->createAnt();
    anth->createAnt();
    assert(anth->getEnergy() == 1499);
    //Check using breakpoint that there are 5 ants at (32, 32) of colony 3
    
    cout << "Passed all tests" << endl;
} */