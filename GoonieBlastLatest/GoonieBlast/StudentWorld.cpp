#include "StudentWorld.h"
#include <string>
#include "Actor.h"
#include <sstream>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(std::string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


//Cosntructor
StudentWorld:: StudentWorld (std::string assetDir): GameWorld(assetDir), m_subLevel(0)
{}

//Destructor
StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld:: init ()
{
    //Max level number is 99, player wins automatically after level 99
    if (getLevel() > 99)
        return GWSTATUS_PLAYER_WON;
    
    //create an Level object and load in current main level and all sub levels
    Level lev(assetDirectory());
    
    //load main level
    ostringstream level;
    
    if(getLevel() < 10)
        level << "level0" << getLevel() << ".dat";
    else
        level << "level" << getLevel() << ".dat";
    
    string curLevel = level.str();
    
    Level::LoadResult result = lev.loadLevel(curLevel);
    
    if (result == Level::load_fail_file_not_found)
    {
        cerr << curLevel << " not found!" << endl;
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level:: load_fail_bad_format)
    {
        cerr << curLevel << " bad format!" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    else if (result == Level:: load_success)
    {
        cerr << curLevel << " loaded successfully." << endl;
    }
    
    //load sub levels
    for (int k = 1; k < MAX_SUB_LEVEL; k++)
    {
        ostringstream level;
        
        if(getLevel() < 10)
            level << "level0" << getLevel() << "_" << k << ".dat";
        else
            level << "level" << getLevel() << "_" << k << ".dat";
        
        string curLevel = level.str();
        
        Level::LoadResult result = lev.loadLevel(curLevel, k);
        
        if (result == Level::load_sublevel_fail_file_not_found)
        {
            cerr << curLevel << " not found!" << endl;
            break;
        }
        else if (result == Level:: load_sublevel_fail_bad_format)
        {
            cerr << curLevel << " bad format!" << endl;
            return GWSTATUS_LEVEL_ERROR;
        }
        else if (result == Level:: load_sublevel_success)
        {
            cerr << curLevel << " loaded successfully." << endl;
        }
    }
    
    //create objects according to the stored info in lev,
    //create objects accordingly with the right coordinate and sub level,
    //and then push each object into the Actor container
    for (int k = 0; k < MAX_SUB_LEVEL; k++)
    {
        vector<Actor*> temp;
        for (int i = 0; i < VIEW_WIDTH; i++)
        {
            for (int j = 0; j < VIEW_HEIGHT; j++)
            {
                Level::MazeEntry item = lev.getContentsOf(i, j, k);
                switch (item)
                {
                    case Level::player:
                    {
                        Player *p = new Player (IID_PLAYER, i, j, Actor::right, this, k);
                        m_player.push_back(p);
                        break;
                    }
                    case Level::wall:
                    {
                        Wall *w = new Wall (IID_WALL, i, j, Actor::none, this, k);
                        temp.push_back(w);
                        break;
                    }
                    case Level::gate0:
                    {
                        Gate *g0 = new Gate(IID_GATE, i, j, Actor::none, this, k, 0);
                        temp.push_back(g0);
                        break;
                    }
                    case Level::gate1:
                    {
                        Gate *g1 = new Gate(IID_GATE, i, j, Actor::none, this, k, 1);
                        temp.push_back(g1);
                        break;
                    }
                    case Level::gate2:
                    {
                        Gate *g2 = new Gate(IID_GATE, i, j, Actor::none, this, k, 2);
                        temp.push_back(g2);
                        break;
                    }
                    case Level::gate3:
                    {
                        Gate *g3 = new Gate(IID_GATE, i, j, Actor::none, this, k, 3);
                        temp.push_back(g3);
                        break;
                    }
                    case Level::gate4:
                    {
                        Gate *g4 = new Gate(IID_GATE, i, j, Actor::none, this, k, 4);
                        temp.push_back(g4);
                        break;
                    }
                    case Level::gate5:
                    {
                        Gate *g5 = new Gate(IID_GATE, i, j, Actor::none, this, k, 5);
                        temp.push_back(g5);
                        break;
                    }
                    case Level::ammo:
                    {
                        Ammo *a = new Ammo(IID_AMMO, i, j, Actor::none, this, k);
                        temp.push_back(a);
                        break;
                    }
                    case Level::jewel:
                    {
                        Jewel *je = new Jewel(IID_JEWEL, i, j, Actor::none, this, k);
                        temp.push_back(je);
                        break;
                    }
                    case Level::extra_life:
                    {
                        ExtraLife *e = new ExtraLife(IID_EXTRA_LIFE, i, j, Actor::none, this, k);
                        temp.push_back(e);
                        break;
                    }
                    case Level::hostage:
                    {
                        Hostage *e = new Hostage(IID_EXTRA_LIFE, i, j, Actor::none, this, k);
                        temp.push_back(e);
                        break;
                    }
                    case Level::restore_health:
                    {
                        RestoreHealth *r = new RestoreHealth(IID_EXTRA_LIFE, i, j, Actor::none, this, k);
                        temp.push_back(r);
                        break;
                    }
                    case Level::exit:
                    {
                        Exit *ex = new Exit(IID_EXTRA_LIFE, i, j, Actor::none, this, k);
                        temp.push_back(ex);
                        break;
                    }
                    case Level::vert_snarlbot:
                    {
                        SnarlBot *vs = new SnarlBot(IID_SNARLBOT, i, j, Actor::down, this, k);
                        temp.push_back(vs);
                        break;
                    }
                    case Level::horiz_snarlbot:
                    {
                        SnarlBot *vs = new SnarlBot(IID_SNARLBOT, i, j, Actor::right, this, k);
                        temp.push_back(vs);
                        break;
                    }
                    case Level::kleptobot_factory:
                    {
                        KleptoBotFactory *kf = new KleptoBotFactory(IID_ROBOT_FACTORY, i, j, Actor::none, this, k);
                        temp.push_back(kf);
                        break;
                    }
                    case Level::empty:
                    default:
                        break;
                }
            }
        }
        ObjectList.push_back(temp);
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld:: move()
{
    //Update screen display of game statuses
    updateDisplay();
    
    //make each alive actor doSomething including player
    int size = (int)ObjectList[m_subLevel].size();
    for (int i = 0; i < size; i++)
    {
        Actor* a = ObjectList[m_subLevel][i];
        if (a->isAlive())
        {
            if (a->getID() == IID_GATE && isPlayeratLocation(a->getX(), a->getY()))
            {
                a->doSomething();
                break;
            }
            a->doSomething();
        }
    }
    
    m_player[m_subLevel]->doSomething();
 
    //remove dead actors from the game
    for (int i = 0; i < ObjectList.size(); i++)
    {
        for (int j = 0; j < ObjectList[i].size(); j++)
        {
            Actor* a2 = ObjectList[i][j];
            if(!a2->isAlive())
            {
                a2->setVisible(false);
                delete a2;
                ObjectList[i].erase(ObjectList[i].begin()+j);
            }
        }
    }
    
    //reduce time limit if time limit is above 0
    if (m_timeLimit > 0)
        m_timeLimit--;
    
    //if time limit runs out or player hit points falls to 0 or below 0
    if (m_timeLimit <= 0 || m_playerHitPoints <= 0)
    {
        //reset sub level to 0, decrease player lives by 1, reset time limit, player hit points and ammo
        //then return GWSTATUS_PLAYER_DIED
        m_subLevel = 0;
        decLives();
        m_timeLimit = 1500;
        m_playerAmmo = 0;
        m_playerHitPoints = 20;
        return GWSTATUS_PLAYER_DIED;
    }

    //if level is completed
    if (isLevelCompleted())
    {
        //inscrease scores by the remaining time limit,
        //reset time limit, player ammo, player hit points and m_levelCompleted,
        //then return GWSTATUS_FINISHED_LEVEL
        increaseScore(m_timeLimit);
        m_timeLimit = 1500;
        m_playerAmmo = 0;
        m_playerHitPoints = 20;
        m_levelCompleted = false;
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld:: cleanUp()
{
    //deallocate and erase all actors from vector containers
    for (int i = 0 ; i < m_player.size(); i++)
    {
        Player* p = m_player[i];
        delete p;
    }

    m_player.clear();

    for (int i = 0; i < ObjectList.size(); i++)
    {
        for (int j = 0; j < ObjectList[i].size(); j++)
        {
            Actor* a = ObjectList[i][j];
            delete a;
        }
        ObjectList[i].clear();
    }
    
    ObjectList.clear();
}

Actor* StudentWorld:: getObject(int x, int y)
{
    //check if player is at given coordinate
    Player* p = m_player[m_subLevel];
    if (p->getX() == x && p->getY() == y)
    {
        return p;
    }
    
    //check if any actor is at given coordinate
    for (int i = 0; i < ObjectList[m_subLevel].size(); i++)
    {
        Actor* a = ObjectList[m_subLevel][i];
        if (a->getX() == x && a->getY() == y)
        {
            return a;
        }
    }
    
    return nullptr;
}

void StudentWorld::setSubLevel(int subLevel)
{
    m_subLevel = subLevel;
}

bool StudentWorld::isPlayeratLocation(int x, int y)
{
    //check to see if player's coordinate matches give cooridinate
    if (m_player[m_subLevel]->getX() == x && m_player[m_subLevel]->getY() == y)
        return true;
    else
        return false;
}

void StudentWorld::incPlayerAmmo()
{
    m_playerAmmo += 25;
}

void StudentWorld::decPlayerAmmo()
{
    --m_playerAmmo;
}

int StudentWorld::getPlayerAmmo()
{
    return m_playerAmmo;
}

void StudentWorld::updateDisplay()
{
    //takes in sctore, level, lives, ammo, time limit, player health
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int ammo = m_playerAmmo;
    unsigned int bonus = m_timeLimit;
    int health = m_playerHitPoints * 100/20;
    
    ostringstream scoreStream;
    scoreStream << "Score: " << setw(7);
    scoreStream.fill('0');
    scoreStream << score;
    
    ostringstream levelStream;
    if(getLevel() < 10)
        levelStream << "level: 0" << level << "_0" << m_subLevel;
    else
        levelStream << "level: " << level << "_0" << m_subLevel;
    
    ostringstream livesStream;
    livesStream << "Lives: " << setw(2);
    livesStream.fill(' ');
    livesStream << lives;
    
    ostringstream healthStream;
    healthStream << "Health: " << setw(3);
    healthStream.fill(' ');
    healthStream << health;
    healthStream << "%";
    
    ostringstream ammoStream;
    ammoStream << "Ammo: " << setw(3);
    ammoStream.fill(' ');
    ammoStream << ammo;
    
    ostringstream timeLimitStream;
    timeLimitStream << "TimeLimit: " << setw(4);
    timeLimitStream.fill(' ');
    timeLimitStream << bonus;
    
    //Display items with right spacing
    string s = scoreStream.str() + "  " + levelStream.str() + "  " + livesStream.str() + "  " + healthStream.str() + "  " + ammoStream.str() + "  " + timeLimitStream.str();
    setGameStatText(s);
}

void StudentWorld::restoreHealth()
{
    m_playerHitPoints = 20;
}

void StudentWorld::setLevelCompleted(bool finish)
{
    m_levelCompleted = finish;
}
bool StudentWorld::isLevelCompleted()
{
    return m_levelCompleted;
}

void StudentWorld:: addActor(Actor *k)
{
    ObjectList[m_subLevel].push_back(k);
}

int StudentWorld::getPlayerHealth()
{
    return m_playerHitPoints;
}

void StudentWorld::setPlayerHealth(int health)
{
    m_playerHitPoints = health;
}

bool StudentWorld::canPlayerMove(int x, int y)
{
    //initialize blocksPlayer to false
    bool blocksPlayer = false;
    
    for(int i = 0; i < ObjectList[m_subLevel].size(); i++)
    {
        //if there is any object at the given coordinate that blocks the player, set blocks player to true
        if(ObjectList[m_subLevel][i]->getX() == x && ObjectList[m_subLevel][i]->getY() == y)
        {
            if(ObjectList[m_subLevel][i]->isBlocked())
                blocksPlayer = true;
        }
    }
    
    //if blocksPlayer is true, return false
    if(blocksPlayer)
        return false;
    
    //Otherwise, return true
    return true;
}


bool StudentWorld::canBotMove(int x, int y)
{
    //If player is at the given cooordinate, return false
    if(m_player[m_subLevel]->getX() == x && m_player[m_subLevel]->getY() == y)
    {
        return false;
    }
    
    //If any actor that blocks the bot is at given coordinate, return false
    for(int i = 0; i < ObjectList[m_subLevel].size(); i++)
    {
        if(ObjectList[m_subLevel][i]->getX() == x && ObjectList[m_subLevel][i]->getY() == y)
        {
            if(ObjectList[m_subLevel][i]->getID() == IID_WALL || ObjectList[m_subLevel][i]->getID() == IID_SNARLBOT || ObjectList[m_subLevel][i]->getID() == IID_KLEPTOBOT || ObjectList[m_subLevel][i]->getID() == IID_ROBOT_FACTORY)
            {
                return false;
            }
        }
    }
    //otherwise, return true
    return true;
}

bool StudentWorld::shouldBotFireBullet(SnarlBot* bot)
{
    //If player is on the same y coordinate
    if((m_player[m_subLevel]->getY() == bot->getY()))
    {
        //if bot is facing left and player's x coordinate is less than bot's
        if(bot->getDirection() == Actor::left && m_player[m_subLevel]->getX() < bot->getX())
        {
            //check if there is any other actor that can block the player from the bullet
            for(int x = bot->getX()-1; x >= m_player[m_subLevel]->getX(); x--)
            {
                Actor* temp = getObject(x,bot->getY());
                
                if(temp != nullptr && (temp->getID() == IID_WALL || temp->getID() == IID_SNARLBOT || temp->getID() == IID_KLEPTOBOT || temp->getID() == IID_ROBOT_FACTORY))
                {
                    return false;
                }
            }
            return true;
        }
        //if bot is facing right and player's x coordinate is greater than bot's
        else if(bot->getDirection() == Actor::right && m_player[m_subLevel]->getX() > bot->getX())
        {
            //check if there is any other actor that can block the player from the bullet
            for(int x = bot->getX()+1; x<=m_player[m_subLevel]->getX(); x++)
            {
                Actor* temp = getObject(x,bot->getY());
                
                if(temp != nullptr && (temp->getID() == IID_WALL || temp->getID() == IID_SNARLBOT || temp->getID() == IID_KLEPTOBOT || temp->getID() == IID_ROBOT_FACTORY))
                {
                    return false;
                }
            }
            return true;
        }
        
        return false;
    }
    
    //If player is on the same x coordinate
    else if((m_player[m_subLevel]->getX() == bot->getX()))
    {
        //if bot is facing down and player's y coordinate is less than bot's
        if(bot->getDirection() == Actor::down && m_player[m_subLevel]->getY() < bot->getY())
        {
            //check if there is any other actor that can block the player from the bullet
            for(int y = bot->getY()-1; y>=m_player[m_subLevel]->getY(); y--)
            {
                Actor* temp = getObject(bot->getX(),y);
                
                if(temp != nullptr && (temp->getID() == IID_WALL || temp->getID() == IID_SNARLBOT || temp->getID() == IID_KLEPTOBOT || temp->getID() == IID_ROBOT_FACTORY))
                {
                    return false;
                }
            }
            return true;
        }
        
        //if bot is facing up and player's y coordinate is greater than bot's
        else if(bot->getDirection() == Actor::up && m_player[m_subLevel]->getY() > bot->getY())
        {
            //check if there is any other actor that can block the player from the bullet
            for(int y = bot->getY()+1; y<=m_player[m_subLevel]->getY(); y++)
            {
                Actor* temp = getObject(bot->getX(),y);
                
                if(temp != nullptr && (temp->getID() == IID_WALL || temp->getID() == IID_SNARLBOT || temp->getID() == IID_KLEPTOBOT || temp->getID() == IID_ROBOT_FACTORY))
                {
                    return false;
                }
            }
            return true;
        }
        
        return false;
    }
    
    
    return false;
}

bool StudentWorld::anyJewelsLeft()
{
    //go through the actor container to see if there is any jewel object
    for (int i = 0; i < ObjectList.size(); i++)
    {
        for (int j = 0; j < ObjectList[i].size(); j++)
        {
            Actor* a2 = ObjectList[i][j];
            if (a2->getID() == IID_JEWEL)
                return true;
        }
    }
    return false;
}

bool StudentWorld::anyHostagesLeft()
{
    //go through the actor container to see if there is any hostage object
    for (int i = 0; i < ObjectList.size(); i++)
    {
        for (int j = 0; j < ObjectList[i].size(); j++)
        {
            Actor* a2 = ObjectList[i][j];
            if (a2->getID() == IID_HOSTAGE)
                return true;
        }
    }
    return false;
}

Actor* StudentWorld:: isActoratLocationAttackable(int x, int y, int IID)
{
    //check if player is at given coordinate
    Player* p = m_player[m_subLevel];
    if (p->getX() == x && p->getY() == y)
    {
        return p;
    }
    
    //check if any actor is at given coordinate
    for (int i = 0; i < ObjectList[m_subLevel].size(); i++)
    {
        //check if actor at given location is attackable and not robot factory
        Actor* a = ObjectList[m_subLevel][i];
        if (a->getX() == x && a->getY() == y && a->getID() != IID)
        {
            if (a->getID() == IID_PLAYER || a->getID() == IID_KLEPTOBOT || a->getID() == IID_SNARLBOT || a->getID() == IID_WALL)
            {
                return a;
            }
        }
    }
    
    //check if any actor is at given coordinate
    for (int i = 0; i < ObjectList[m_subLevel].size(); i++)
    {
        //check if actor at given location is robot factory
        Actor* a2 = ObjectList[m_subLevel][i];
        if (a2->getX() == x && a2->getY() == y && a2->getID() != IID)
        {
            if (a2->getID() == IID_ROBOT_FACTORY)
            {
                return a2;
            }
        }
    }
    
    return nullptr;
}

