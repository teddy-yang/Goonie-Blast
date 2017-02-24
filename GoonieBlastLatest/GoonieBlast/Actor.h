#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual ~Actor() = 0;
    virtual void doSomething() = 0;
    StudentWorld* getWorld() ;
    virtual bool isBlocked();
    virtual void damage() {return;};
    bool isAlive();
    void setAliveorDead(bool aliveorDead);
    virtual int getHealth() {return 0;};
    virtual void setHealth(int health) {return;};
    
private:
    bool m_alive;
    StudentWorld* m_world;
};

class AttackableActor: public Actor
{
public:
    AttackableActor(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual bool isBlocked();
};

class Robot: public AttackableActor
{
public:
    Robot(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    void setCurrentTick(int n);
    int getCurrentTick();
    int getTicks();
    virtual void damage() = 0;
    virtual void setHealth(int health);
    virtual int getHealth();
private:
    int m_health;
    int m_ticks;
    int m_currentTicks;
};

class NonAttackableActor: public Actor
{
public:
    NonAttackableActor(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
};

class SnarlBot: public Robot
{
public:
    SnarlBot(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
    virtual void damage();
};

class KleptoBot: public Robot
{
public:
    KleptoBot(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
    virtual void damage();
    
private:
    int m_distanceBeforeTurning = rand() % 3 + 1;
    int m_goodieID = -1;
    int m_movedDistance = 0;
    bool consideredRight = false;
    bool consideredLeft = false;
    bool consideredUp = false;
    bool consideredDown = false;
};

class Bullet: public NonAttackableActor
{
public:
    Bullet(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
    void moveWithDirection();
};

class Gate: public NonAttackableActor
{
public:
    Gate(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level, int gateNum);
    virtual void doSomething();
private:
    int m_toSub;
};

class Player: public AttackableActor
{
public:
    Player (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething ();
    virtual void damage();
};

class PickupableActor: public NonAttackableActor
{
public:
    PickupableActor(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
};

class Ammo: public PickupableActor
{
public:
    Ammo(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
};

class ExtraLife: public PickupableActor
{
public:
    ExtraLife(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
};

class Jewel: public PickupableActor
{
public:
    Jewel(int IID, int x, int y, Direction dir, StudentWorld*studWorld, int sub_level);
    virtual void doSomething();
};

class RestoreHealth: public PickupableActor
{
public:
    RestoreHealth(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
};

class Hostage: public PickupableActor
{
public:
    Hostage(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
};

class Exit: public NonAttackableActor
{
public:
    Exit(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
};

class Wall: public NonAttackableActor
{
public:
    Wall (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
    virtual bool isBlocked();
};

class KleptoBotFactory: public NonAttackableActor
{
public:
    KleptoBotFactory(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level);
    virtual void doSomething();
    virtual bool isBlocked();
private:
    int m_count;
};

#endif // ACTOR_H_
