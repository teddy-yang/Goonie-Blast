#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////
//Actor Class//
///////////////

//Constructor
Actor:: Actor (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) : GraphObject (IID, x, y, dir, sub_level), m_world(studWorld)
{
    //initialize as visible and alive
    setVisible(true);
    m_alive = true;
}

//Destructor
Actor::~Actor()
{
    setVisible(false);
}

//Helper function, returns current world the Actor is in
StudentWorld* Actor:: getWorld()
{
    return m_world;
}

//returns true if the Actor blocks other Actors, set to return false
bool Actor:: isBlocked()
{
    return false;
}

//returns true if Actor is alive
bool Actor:: isAlive()
{
    return m_alive;
}

//set actor status to alive or dead
void Actor::setAliveorDead(bool aliveorDead)
{
    m_alive = aliveorDead;
}

///////////////////
//AttackableActor//
///////////////////


//Constructor
AttackableActor::AttackableActor(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) :  Actor(IID, x, y, dir, studWorld, sub_level)
{}

//returns true if the Actor blocks other Actors, set to return true
bool AttackableActor::isBlocked()
{
    return true;
}


//////////////////////
//NonAttackableActor//
//////////////////////

//Constructor
NonAttackableActor::NonAttackableActor(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) :  Actor(IID, x, y, dir, studWorld, sub_level)
{}

/////////
//Robot//
/////////

//Constructor
Robot::Robot(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level): AttackableActor(IID, x, y, dir, studWorld, sub_level)
{
    //initialize current ticks to 0
    m_currentTicks = 0;
    
    //determine ticks with give formula
    m_ticks = (28-getWorld()->getLevel()) / 4;
    if(m_ticks < 3)
        m_ticks = 3;
}

//set current tick to an int value
void Robot::setCurrentTick(int n)
{
    m_currentTicks = n;
}

//returns current ticks
int Robot::getCurrentTick()
{
    return m_currentTicks;
}

//returns ticks
int Robot::getTicks()
{
    return m_ticks;
}

//set health to an int value
void Robot::setHealth(int health)
{
    m_health = health;
}

//returns health
int Robot::getHealth()
{
    return m_health;
}

////////////
//SnarlBot//
////////////


//Constructor
SnarlBot::SnarlBot(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level): Robot(IID, x, y, dir, studWorld, sub_level)
{
    //initialize with health set to 15
    setHealth(15);
}

void SnarlBot::doSomething()
{
    //if dead, do nothing
    if(!isAlive())
    {
        return;
    }
    
    //if current tick is the correct tick
    else if(getCurrentTick() == getTicks())
    {
        //check if SnarlBot should fire, if so
        if(getWorld()->shouldBotFireBullet(this))
        {
            //create a new bullet object, add it to container then play sound
            Bullet *b = new Bullet(IID_BULLET, getX(), getY(), this->getDirection(), this->getWorld(), this->getWorld()->getCurrentSubLevel());
            getWorld()->addActor(b);
            getWorld()->playSound(SOUND_ENEMY_FIRE);
        }
        
        //If not
        else
        {
            //get direction, check if Snarlbot should move then move accordingly
            switch (getDirection())
            {
                case up:
                    if(getWorld()->canBotMove(getX(), getY()+1))
                        moveTo(getX(), getY()+1);
                    else
                        setDirection(down);
                    break;
                case down:
                    if(getWorld()->canBotMove(getX(), getY()-1))
                        moveTo(getX(), getY()-1);
                    else
                        setDirection(up);
                    break;
                case left:
                    if(getWorld()->canBotMove(getX()-1, getY()))
                        moveTo(getX()-1, getY());
                    else
                        setDirection(right);
                    break;
                case right:
                    if(getWorld()->canBotMove(getX()+1, getY()))
                        moveTo(getX()+1, getY());
                    else
                        setDirection(left);
                    break;
                default:
                    break;
            }
            
        }
        //reset current tick to 0
        setCurrentTick(0);
    }
    
    //Current tick is not the right tick
    else
    {
        //increase current tick by 1
        setCurrentTick(getCurrentTick()+1);
    }
}

void SnarlBot::damage()
{
    //reduce health by 3
    setHealth(getHealth() - 3);
    
    //if health is below 0, set SnarlBot to dead and play sound
    if (getHealth() <= 0)
    {
        setAliveorDead(false);
        getWorld()->increaseScore(200);
        getWorld()->playSound(SOUND_ROBOT_DIE);
    }
    
    //otherwise, play impact sound
    else
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

/////////////
//KleptoBot//
/////////////

//Constructor
KleptoBot::KleptoBot(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level): Robot(IID = IID_KLEPTOBOT, x, y, dir, studWorld, sub_level)
{
    //initialize with health set to 9
    setHealth(9);
}

void KleptoBot::doSomething()
{
    //if dead, do nothing
    if (!isAlive())
    {
        return;
    }
    
    //if current tick is not the correct tick, increase current tick by 1
    if (getCurrentTick() != getTicks())
    {
        setCurrentTick(getCurrentTick()+1);
    }
    
    else
    {
        //reset current tick to 0
        setCurrentTick(0);
        
        //check if KelptoBot is on a pickupable goodie, then pick it up accordingly to the probability
        Actor* a = getWorld()->getObject(getX(), getY());
        if (a != nullptr && (a->getID() == IID_EXTRA_LIFE || a->getID() == IID_AMMO || a->getID() == IID_RESTORE_HEALTH) && m_goodieID == -1)
        {
            if (rand() % 5 == 1)
            {
                a->setAliveorDead(false);
                getWorld()->playSound(SOUND_ROBOT_MUNCH);
                m_goodieID = a->getID();
                return;
            }
        }
        
        //get direction and set dx and dy accordingly
        int dx = 0; int dy = 0;
        switch (getDirection())
        {
            case GraphObject::right:
                dx = 1;
                break;
            case GraphObject::left:
                dx = -1;
                break;
            case GraphObject::up:
                dy = 1;
                break;
            case GraphObject::down:
                dy = -1;
                break;
            default:
                break;
        }
        
        //check if there is a object that blocks the KleptoBot and if the KleptoBot has reached distance before turning
        //then move accordingly
        Actor* afterActor = getWorld()->getObject(getX()+dx, getY()+dy);
        
        if ((m_movedDistance != m_distanceBeforeTurning) && ((afterActor == nullptr) || !afterActor->isBlocked()))
        {
            moveTo(getX()+dx, getY()+dy);
            m_movedDistance++;
            return;
        }
        
        else
        {
            //generate random distance before turning
            m_distanceBeforeTurning = rand() % 3 + 1;
            m_movedDistance = 0;
            
            //generate random direction
            Direction randDir;
            int newDir = rand() % 4;
            if (newDir == 0)
                randDir = right;
            else if (newDir == 1)
                randDir = left;
            else if (newDir == 2)
                randDir = up;
            else if (newDir == 3)
                randDir =  down;
            else
                randDir = none;
            
            //set directiont to the generated random direction
            setDirection(randDir);
            
            //consider directions and move accordingly
            while (consideredRight == false || consideredLeft == false || consideredUp == false || consideredDown == false)
            {
                if (getDirection() == right)
                {
                    int dx2 = 0; int dy2 = 0;
                    switch (getDirection())
                    {
                        case GraphObject::right:
                            dx2 = 1;
                            break;
                        case GraphObject::left:
                            dx2 = -1;
                            break;
                        case GraphObject::up:
                            dy2 = 1;
                            break;
                        case GraphObject::down:
                            dy2 = -1;
                            break;
                        default:
                            break;
                    }
                    
                    Actor* afterActor2 = getWorld()->getObject(getX()+dx2, getY()+dy2);
                    
                    if (afterActor2 == nullptr || !afterActor2->isBlocked())
                    {
                        moveTo(getX()+dx2, getY()+dy2);
                        return;
                    }
                    else
                    {
                        consideredRight = true;
                        setDirection(left);
                        return;
                    }
                }
                
                if (getDirection() == left)
                {
                    int dx2 = 0; int dy2 = 0;
                    switch (getDirection())
                    {
                        case GraphObject::right:
                            dx2 = 1;
                            break;
                        case GraphObject::left:
                            dx2 = -1;
                            break;
                        case GraphObject::up:
                            dy2 = 1;
                            break;
                        case GraphObject::down:
                            dy2 = -1;
                            break;
                        default:
                            break;
                    }
                    
                    Actor* afterActor2 = getWorld()->getObject(getX()+dx2, getY()+dy2);
                    
                    if (afterActor2 == nullptr || !afterActor2->isBlocked())
                    {
                        moveTo(getX()+dx2, getY()+dy2);
                        return;
                    }
                    else
                    {
                        consideredLeft = true;
                        setDirection(up);
                        return;
                    }
                }
                if (getDirection() == up)
                {
                    int dx2 = 0; int dy2 = 0;
                    switch (getDirection())
                    {
                        case GraphObject::right:
                            dx2 = 1;
                            break;
                        case GraphObject::left:
                            dx2 = -1;
                            break;
                        case GraphObject::up:
                            dy2 = 1;
                            break;
                        case GraphObject::down:
                            dy2 = -1;
                            break;
                        default:
                            break;
                    }
                    
                    Actor* afterActor2 = getWorld()->getObject(getX()+dx2, getY()+dy2);
                    
                    if (afterActor2 == nullptr || !afterActor2->isBlocked())
                    {
                        moveTo(getX()+dx2, getY()+dy2);
                        return;
                    }
                    else
                    {
                        consideredUp = true;
                        setDirection(down);
                        return;
                    }
                }
                if (getDirection() == down)
                {
                    int dx2 = 0; int dy2 = 0;
                    switch (getDirection())
                    {
                        case GraphObject::right:
                            dx2 = 1;
                            break;
                        case GraphObject::left:
                            dx2 = -1;
                            break;
                        case GraphObject::up:
                            dy2 = 1;
                            break;
                        case GraphObject::down:
                            dy2 = -1;
                            break;
                        default:
                            break;
                    }
                    
                    Actor* afterActor2 = getWorld()->getObject(getX()+dx2, getY()+dy2);
                    
                    if (afterActor2 == nullptr || !afterActor2->isBlocked())
                    {
                        moveTo(getX()+dx2, getY()+dy2);
                        return;
                    }
                    else
                    {
                        consideredDown = true;
                        setDirection(right);
                        return;
                    }
                }
            }
            
            //If all directions are blocked set direction to the generated random direction
            setDirection(randDir);
        }
    }
}

void KleptoBot::damage()
{
    //reduce health by 3
    setHealth(getHealth() - 3);
    
    //if health drops below 0
    if (getHealth() <= 0)
    {
        //set to dead and play sound, if picked up goodie drop it at KleptoBot's location
        setAliveorDead(false);
        getWorld()->increaseScore(20);
        getWorld()->playSound(SOUND_ROBOT_DIE);
        if(m_goodieID == IID_RESTORE_HEALTH)
        {
            RestoreHealth *droppedGoodie = new RestoreHealth(IID_RESTORE_HEALTH, getX(),getY(), none, getWorld(), getWorld()->getCurrentSubLevel());
            getWorld()->addActor(droppedGoodie);
        }
        else if(m_goodieID == IID_EXTRA_LIFE)
        {
            ExtraLife *droppedGoodie = new ExtraLife(IID_EXTRA_LIFE, getX(),getY(), none, getWorld(), getWorld()->getCurrentSubLevel());
            getWorld()->addActor(droppedGoodie);
        }
        else if(m_goodieID == IID_AMMO)
        {
            Ammo *droppedGoodie = new Ammo(IID_AMMO, getX(),getY(), none, getWorld(), getWorld()->getCurrentSubLevel());
            getWorld()->addActor(droppedGoodie);
        }
    }
    
    //Otherwise, just play impact sound
    else
    {
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    }
}

//////////
//Bullet//
//////////

//Constructor
Bullet::Bullet(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level): NonAttackableActor(IID = IID_BULLET, x, y, dir, studWorld, sub_level)
{
    if (dir == right)
    {
        moveTo(getX()+1, getY());
    }
    else if (dir == left)
    {
        moveTo(getX()-1, getY());
    }
    else if (dir == up)
    {
        moveTo(getX(), getY()+1);
    }
    else if (dir == down)
    {
        moveTo(getX(), getY()-1);
    }
}

void Bullet:: doSomething()
{
    //if dead, do nothing
    if (!isAlive())
        return;
    
    //If there is any actor at bullet’s location that blocks the bullet,
    //deal damage accordingly and set to dead then return.
    Actor* a = getWorld()->isActoratLocationAttackable(getX(), getY(), IID_BULLET);
    if (a != nullptr)
    {
        a->damage();
        setAliveorDead(false);
        return;
    }
    
    //move according to bullet's direction
    moveWithDirection();
    
    //If there is any actor at bullet’s location that blocks the bullet,
    //deal damage accordingly and set to dead then return.
    Actor* a2 = getWorld()->isActoratLocationAttackable(getX(), getY(), IID_BULLET);
    if (a2 != nullptr)
    {
        a2->damage();
        setAliveorDead(false);
        return;
    }
}

void Bullet::moveWithDirection()
{
    //get bullet's direction and move accordingly
    switch (getDirection())
    {
        case up:
            moveTo(getX(), getY()+1);
            break;
        case down:
            moveTo(getX(), getY()-1);
            break;
        case left:
            moveTo(getX()-1, getY());
            break;
        case right:
            moveTo(getX()+1, getY());
            break;
        default:
            break;
    }

}

////////
//Gate//
////////

//Constructor initializes gates with correct gate number stored in m_toSub
Gate::Gate(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level, int gateNum): NonAttackableActor(IID = IID_GATE, x, y, dir = none, studWorld, sub_level), m_toSub(gateNum)
{}

void Gate::doSomething()
{
    //if dead, do nothing
    if (!isAlive())
        return;

    //check if player is on gate
    Actor* a = getWorld()->getObject(getX(), getY());
    if (a->getID() == IID_PLAYER)
    {
        //change sub level and set gate to dead
        getWorld()->setSubLevel(m_toSub);
        setAliveorDead(false);
    }
}

//////////
//Player//
//////////

//Constructor
Player:: Player (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level): AttackableActor(IID = IID_PLAYER, x , y, dir = right, studWorld, sub_level)
{}

void Player::doSomething()
{
    //if dead, do nothing
    if (!isAlive())
    {
        return;
    }
    
    //takes in a key entry then execute accordingly
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_ESCAPE:
                setAliveorDead(false);
                getWorld()->setPlayerHealth(0);
                break;
            case KEY_PRESS_SPACE:
                if (getWorld()->getPlayerAmmo() > 0)
                {
                    Bullet *b = new Bullet(IID_BULLET, getX(), getY(), this->getDirection(), this->getWorld(), this->getWorld()->getCurrentSubLevel());
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    getWorld()->decPlayerAmmo();
                    getWorld()->addActor(b);
                }
                break;
            case KEY_PRESS_LEFT:
            {
                setDirection(left);
                if(getWorld()->canPlayerMove(getX()-1,getY()))
                    moveTo(getX()-1, getY());
                break;
            }
                
            case KEY_PRESS_RIGHT:
            {
                setDirection(right);
                if(getWorld()->canPlayerMove(getX()+1,getY()))
                    moveTo(getX()+1, getY());
                break;
            }
                
            case KEY_PRESS_UP:
            {
                setDirection(up);
                if(getWorld()->canPlayerMove(getX(),getY()+1))
                    moveTo(getX(), getY()+1);
                break;
            }
                
            case KEY_PRESS_DOWN:
            {
                setDirection(down);
                if(getWorld()->canPlayerMove(getX(),getY()-1))
                    moveTo(getX(), getY()-1);
                break;
            }
            default:
                break;
        }
    }
}

void Player::damage()
{
    //reduce player health by 3
    getWorld()->setPlayerHealth(getWorld()->getPlayerHealth() - 3);
    
    //if player health is above 0, play imapact sound
    if (getWorld()->getPlayerHealth() > 0)
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    
    //Otherwise set to dead and play player die sound
    else
    {
        setAliveorDead(false);
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
}

///////////////////
//PickupableActor//
///////////////////

//Constructor
PickupableActor::PickupableActor(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level): NonAttackableActor(IID, x, y, dir = none, studWorld, sub_level)
{}

////////
//Ammo//
////////

//Constructor
Ammo::Ammo (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) : PickupableActor(IID  = IID_AMMO, x, y, dir = none, studWorld, sub_level)
{}

void Ammo::doSomething()
{
    //if dead, do nothing
    if (!isAlive())
        return;
    
    //if player is at the same location
    if (getWorld()->isPlayeratLocation(getX(), getY()))
    {
        //increase player ammo, score accordingly, play sound got goodie then set to dead
        getWorld()->incPlayerAmmo();
        getWorld()->increaseScore(200);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setAliveorDead(false);
    }
}

/////////////
//Extralife//
/////////////

//Constructor
ExtraLife::ExtraLife (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) : PickupableActor(IID = IID_EXTRA_LIFE, x, y, dir = none, studWorld, sub_level)
{}

void ExtraLife::doSomething()
{
    //if dead, do nothing
    if (!isAlive())
        return;
    
    //if player is at the same location
    if (getWorld()->isPlayeratLocation(getX(), getY()))
    {
        //increase player lives, score accordingly, play sound got goodie then set to dead
        getWorld()->incLives();
        getWorld()->increaseScore(500);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setAliveorDead(false);
    }
}

/////////
//Jewel//
/////////

//Constructor
Jewel::Jewel (int IID, int x, int y, Direction dir, StudentWorld *studWorld, int sub_level) : PickupableActor(IID = IID_JEWEL, x , y, dir = none, studWorld, sub_level)
{}

void Jewel::doSomething()
{
    //if dead, do nothing
    if (!isAlive())
        return;
    
    //if player is at the same location
    if (getWorld()->isPlayeratLocation(getX(), getY()))
    {
        //increase score accordingly, play sound got goodie then set to dead
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setAliveorDead(false);
    }
}

/////////////////
//RestoreHealth//
/////////////////

//Constructor
RestoreHealth::RestoreHealth (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) : PickupableActor(IID = IID_RESTORE_HEALTH, x, y, dir = none, studWorld, sub_level)
{}

void RestoreHealth::doSomething()
{
    //if dead, do nothing
    if (!isAlive())
        return;
    
    //if player is at the same location
    if (getWorld()->isPlayeratLocation(getX(), getY()))
    {
        //restore player's hit points, increase score accordingly, play sound got goodie then set to dead
        getWorld()->restoreHealth();
        getWorld()->increaseScore(1000);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setAliveorDead(false);
    }
}

///////////
//Hostage//
///////////

//Constructor
Hostage::Hostage (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) : PickupableActor(IID = IID_HOSTAGE , x, y, dir = none, studWorld, sub_level)
{}

void Hostage::doSomething()
{
    //if dead, do nothing
    if (!isAlive())
        return;
    
    //if player is at the same location
    if (getWorld()->isPlayeratLocation(getX(), getY()))
    {
        //play sound got goodie then set to dead
        getWorld()->playSound(SOUND_GOT_GOODIE);
        setAliveorDead(false);
    }
}

////////
//Wall//
////////

//Constructor
Wall:: Wall (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) : NonAttackableActor(IID = IID_WALL, x, y, dir = none, studWorld, sub_level)
{}

void Wall:: doSomething()
{
    //do nothing
    return;
}

bool Wall:: isBlocked()
{
    return true;
}

////////
//Exit//
////////

//Constructor
Exit::Exit (int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) : NonAttackableActor(IID = IID_EXIT, x, y, dir = none, studWorld, sub_level)
{
    //initialize with status set as invisible
    setVisible(false);
}

void Exit:: doSomething()
{
    //if no jewels or hostages left, exit is invisible
    if (!getWorld()->anyJewelsLeft() && !getWorld()->anyHostagesLeft() && !isVisible())
    {
        //set exit to visible and play sound reveal exit
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
    
    //if player is at the same location
    if (getWorld()->isPlayeratLocation(getX(), getY()) && isVisible())
    {
        //play sound finished level, set level to completed, increase scores accordingly, and set to invisible
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->setLevelCompleted(true);
        getWorld()->increaseScore(1000);
        setVisible(false);
    }
}

////////////////////
//KleptoBotFactory//
///////////////////

//Constructor
KleptoBotFactory::KleptoBotFactory(int IID, int x, int y, Direction dir, StudentWorld* studWorld, int sub_level) : NonAttackableActor(IID = IID_ROBOT_FACTORY, x, y, dir = none, studWorld, sub_level)
{}

void KleptoBotFactory::doSomething()
{
    //initialize count to 0 and establish bounds
    m_count = 0;
    
    int x = getX(), y = getY();
    int upperBound = y + 3;
    int lowerBound = y - 3;
    int leftBound = x - 3;
    int rightBound = x + 3;
    
    if (upperBound >= VIEW_HEIGHT)
        upperBound = VIEW_HEIGHT - 1;
    if (lowerBound <= 0)
        lowerBound = 0;
    if (leftBound <= 0)
        leftBound = 0;
    if (rightBound >= VIEW_WIDTH)
        rightBound = VIEW_WIDTH - 1;
    
    //count KleptoBots inside bounds
    for (int i = lowerBound; i <= upperBound; i++)
    {
        for (int j = leftBound; j <= rightBound; j++)
        {
            if (getWorld()->getObject(i, j))
            {
                if (getWorld()->getObject(i, j)->getID() == IID_KLEPTOBOT)
                {
                    m_count++;
                }
            }
        }
    }

    //if count is less than 3 and there is no kleptobot at the factory
    if (m_count < 3 && getWorld()->getObject(getX(), getY())->getID() != IID_KLEPTOBOT)
    {
        //in 1 in 50 chance
        if (rand() % 50 == 1)
        {
            //create a KleptoBot, add it to the actor continer then play sound robot born
            KleptoBot *k = new KleptoBot(IID_KLEPTOBOT, getX(), getY(), right, getWorld(), getWorld()->getCurrentSubLevel());
            getWorld()->addActor(k);
            getWorld()->playSound(SOUND_ROBOT_BORN);
        }
    }
}

bool KleptoBotFactory::isBlocked()
{
    return true;
}



