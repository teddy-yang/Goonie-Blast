#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include <string>
#include "Level.h"
#include <vector>

class Actor;
class Player;
class SnarlBot;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld: public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual int getCurrentSubLevel() {return m_subLevel;};
    Actor* getObject(int x, int y);
    void setSubLevel(int subLevel);
    bool isPlayeratLocation(int x, int y);
    void incPlayerAmmo();
    void decPlayerAmmo();
    int getPlayerAmmo();
    void updateDisplay();
    void restoreHealth();
    void setLevelCompleted(bool finish);
    bool isLevelCompleted();
    void addActor(Actor *k);
    int getPlayerHealth();
    void setPlayerHealth(int health);
    bool canPlayerMove(int x, int y);
    bool canBotMove(int x, int y);
    bool shouldBotFireBullet(SnarlBot* bot);
    bool anyJewelsLeft();
    bool anyHostagesLeft();
    Actor* isActoratLocationAttackable(int x, int y, int IID);
    
private:
    std::vector<std::vector<Actor*> > ObjectList;
    std::vector<Player*> m_player;
    int m_subLevel;
    int m_playerHitPoints = 20;
    int m_playerAmmo = 0;
    int m_timeLimit = 1500;
    bool m_levelCompleted = false;
};

#endif // STUDENTWORLD_H_
