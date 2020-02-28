#include "StudentWorld.h"
#include "Actor.h"

#include "GameConstants.h"
#include <string>
#include <math.h>

#include<iostream>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
using namespace std;
ostringstream oss;

//#include<cmath>
//atan(x1-x2, y1-y2)
//convert to degrees (pi/180)

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}


// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{
}

double StudentWorld::findEuclidean(double startX, double startY, double endX, double endY) const
{
    return sqrt(((endX - startX) * (endX - startX)) + ((endY - startY) * (endY - startY)));
}

bool StudentWorld::checkAllowed(int startX, int startY)
{
    bool allowed = true;
    for (int k = 0; k < coords.size(); k++)
    {
        if (coords[k].x == startX && coords[k].y == startY)
        {
            allowed = false;
        }
        else
        {
            int d = findEuclidean(startX, startY, coords[k].x, coords[k].y);
            if (d < (2 * SPRITE_RADIUS))
            {
                allowed = false;
            }
        }
    }
    return allowed;
}




void StudentWorld::addActor(Actor* a)
{
    actors.push_back(a);
}

bool StudentWorld::damageOneActor(Actor* a, int damage)
{
    return a->takeDamage(damage);
}


// Is bacterium a blocked from moving to the indicated location?
bool StudentWorld::isBacteriumMovementBlockedAt(Actor* a, double x, double y) const
{
    int r = findEuclidean(x, y, VIEW_WIDTH / 2, VIEW_HEIGHT / 2);
    if (r > VIEW_RADIUS)
    {
        return true;
    }
    list<Actor*>::const_iterator it = actors.begin();
    for (; it != actors.end(); it++)
    {
        if ((*it)->blocksBacteria())
        {
            int actorX = (*it)->getX(), actorY = (*it)->getY();       //DOES NOT GET CORRECT X AND Y
            if (findEuclidean(actorX, actorY, x, y) <= SPRITE_RADIUS)   //NEVER GOES IN
            {
                return true;
            }
        }
    }
    return false;
}

Socrates* StudentWorld::getOverlappingSocrates(Actor* a) const
{
    double socX = m_player->getX();
    double socY = m_player->getY();

    if (findEuclidean(socX, socY, a->getX(), a->getY()) <= SPRITE_RADIUS * 2)
    {
        return m_player;
    }
    return nullptr;
}

Actor* StudentWorld::getOverlappingEdible(Actor* a) const
{
    list<Actor*>::const_iterator it = actors.begin();
    for (; it != actors.end(); it++)
    {
        if ((*it)->isEdible())
        {
            int x = (*it)->getX();
            int y = (*it)->getY();
            if (findEuclidean(x, y, a->getX(), a->getY()) <= (2 * SPRITE_RADIUS))
            {
                return *it;
            }
        }
    }
    return nullptr;
}

bool StudentWorld::getAngleToNearbySocrates(Actor* a, int dist, int& angle) const
{
    double socX = m_player->getX(), socY = m_player->getY();
    if (findEuclidean(socX, socY, a->getX(), a->getY()) <= dist)
    {
        angle = atan2(socX - VIEW_WIDTH / 2, socY - VIEW_HEIGHT / 2);
        return true;
    }
    return false;
}








int StudentWorld::init()
{
    m_player = new Socrates(0, VIEW_HEIGHT / 2, this);

    ///////Pit////////
    for (int p = 0; p < getLevel(); p++)
    {
        int startX = randInt(0, 256);
        int startY = randInt(0, 256);

        int r = findEuclidean(startX, startY, VIEW_WIDTH / 2, VIEW_HEIGHT / 2);
        if (r <= 120 && checkAllowed(startX, startY))
        {
            actors.push_back(new Pit(startX, startY, this));
            coords.push_back(Coordinate(startX, startY));
        }
        else
        {
            p--;
        }

    }

    /////Food/////
    for (int f = 0; f < min(5 * getLevel(), 25); f++)
    {
        int startX = randInt(0, 256);
        int startY = randInt(0, 256);
        int r = findEuclidean(startX, startY, VIEW_WIDTH / 2, VIEW_HEIGHT / 2);
        if (r <= 120 && checkAllowed(startX, startY))
        {
            actors.push_back(new Food(startX, startY, this));
            //coords.push_back(Coordinate(startX, startY));

        }
        else
        {
            f--;
        }

    }

    /////Dirt////////
    int numDirt = max((180 - 2 * getLevel()), 20);
    for (int d = 0; d < numDirt; d++)
    {
        int startX = randInt(0, 256);
        int startY = randInt(0, 256);
        int r = findEuclidean(startX, startY, VIEW_WIDTH / 2, VIEW_HEIGHT / 2);
        if (r <= 120 && checkAllowed(startX, startY))
        {
            actors.push_back(new Dirt(startX, startY, this));
            // coords.push_back(Coordinate(startX, startY));
        }
        else
        {
            d--;
        }
    }

    //string stats = "Score: " + to_string(getScore()) + " Level: " + to_string(getLevel()) + " Lives: " + to_string(getLives()) + " health: " + to_string((*m_player).getHitPoints()) + " Sprays: " + to_string(m_player->getSprays()) + " Flames: " + to_string(m_player->getFlames()) + " size: " + to_string(actors.size());
    //setGameStatText(stats);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // this code is here merely to allow the game to build, run, and terminate after you hit enter.
    // notice that the return value gwstatus_player_died will cause our framework to end the current level.
    
    
    if (m_player->isAlive())
    {
        
        m_player->doSomething();
        list<Actor*>::iterator it = actors.begin();
        for (; it != actors.end();)
        {
            if ((*it)->isAlive())
            {
                (*it)->doSomething();
                it++;
            }
            else if (!(*it)->isAlive())
            {
                delete (*it);
                it = actors.erase(it);
            }
        }
        createGoodies();
    }
    else if (!m_player->isAlive())
    {
        return GWSTATUS_PLAYER_DIED;
    }

    //string stats = "score: " + to_string(getScore()) + " level: " + to_string(getLevel()) + " lives: " + to_string(getLives()) + " health: " + to_string(m_player->getHitPoints()) + " sprays: " + to_string(m_player->getSprays()) + " flames: " + to_string(m_player->getFlames()); // +" size: " + to_string(actors.size());
    //setGameStatText(stats);
    return 1;
}

void StudentWorld::cleanUp()
{
    delete m_player;
    list<Actor*>::iterator it = actors.begin();
    for (; it != actors.end();)
    {
        delete* it;
        it = actors.erase(it);
    }
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::createGoodies()
{
    int chanceFungus = min(510 - getLevel() * 10, 200);
    int rand = randInt(0, chanceFungus - 1);
    int randAngle = randInt(0, 359);
    if (rand == 0)
    {
        double x, y = 0;
        x = VIEW_RADIUS * cos(randAngle) + VIEW_WIDTH / 2;
        y = VIEW_RADIUS * sin(randAngle) + VIEW_WIDTH / 2;
        actors.push_back(new Fungus( x, y, this));
        return;
    }
    else
    {
        int chanceGoodie = min(510 - getLevel() * 10, 200);
        int rand2 = randInt(0, chanceGoodie - 1);
        int randAngle2 = randInt(0, 359);
        if (rand2 == 0)
        {
            double x, y = 0;
            x = VIEW_RADIUS * cos(randAngle2) + VIEW_WIDTH / 2;
            y = VIEW_RADIUS * sin(randAngle2) + VIEW_WIDTH / 2;
            int typeGoodie = randInt(1, 10);
            if (typeGoodie <= 6)
            {
                actors.push_back(new RestoreHealthGoodie(x, y, this));
                return;
            }
            else if (typeGoodie > 6 && typeGoodie <= 9)
            {
                actors.push_back(new FlameThrowerGoodie(x, y, this));
                return;
            }
            else if (typeGoodie == 10)
            {
                actors.push_back(new ExtraLifeGoodie(x, y, this));
                return;
            }
        }
    }
}