#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include<list>
#include<vector>

class Actor;
class Socrates;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    double findEuclidean(double startX, double startY, double endX, double endY) const;
    bool checkAllowed(int startX, int startY);

    // Add an actor to the world.
    void addActor(Actor* a);

    // If actor a ovelaps some live actor, damage that live actor by the
    // indicated amount of damage and return true; otherwise, return false.
    bool damageOneActor(Actor* a, int damage);

    // Is bacterium a blocked from moving to the indicated location?
    bool isBacteriumMovementBlockedAt(Actor* a, double x, double y) const;

    // If actor a overlaps this world's socrates, return a pointer to the
    // socrates; otherwise, return nullptr
    Socrates* getOverlappingSocrates(Actor* a) const;

    // If actor a overlaps a living edible object, return a pointer to the
    // edible object; otherwise, return nullptr
    Actor* getOverlappingEdible(Actor* a) const;

    // Return true if this world's socrates is within the indicated distance
    // of actor a; otherwise false.  If true, angle will be set to the
    // direction from actor a to the socrates.
    bool getAngleToNearbySocrates(Actor* a, int dist, int& angle) const;

    // Return true if there is a living edible object within the indicated
    // distance from actor a; otherwise false.  If true, angle will be set
    // to the direction from actor a to the edible object nearest to it.
    bool getAngleToNearestNearbyEdible(Actor* a, int dist, int& angle) const;
    
    // Set x and y to the position on the circumference of the Petri dish
    // at the indicated angle from the center.  (The circumference is
    // where socrates and goodies are placed.)
    void getPositionOnCircumference(int angle, double& x, double& y) const;

    /*Actor* overlapsProjectile(double projX, double projY)
    {
        std::list<Actor*>::iterator it = actors.begin();
        for(;it != actors.end();it++)
        {
            double x = (*it)->getX();
            double y = (*it)->getY();
            if ((*it)->takeDamage(0))
            {
                if (findEuclidean(projX, projY, x, y) <= SPRITE_RADIUS * 2)
                {
                    return *it;
                }
            }
        }
        return nullptr;
    }*/

    bool overlapsProjectile(double projX, double projY, int damage)
    {
        std::list<Actor*>::iterator it = actors.begin();
        for (; it != actors.end(); it++)
        {
            double x = (*it)->getX();
            double y = (*it)->getY();
            if ((*it)->isDamagable())
            {
                if (findEuclidean(x, y, projX, projY) <= (2 * SPRITE_RADIUS))
                {
                    if ((*it)->isLiveDamagable())
                    {
                       (*it)->decHitPoints(damage);
                       if ((*it)->isAlive())
                       {
                           (*it)->playHurt();
                       }
                        else
                        {
                            (*it)->setDead();
                            (*it)->playDead();
                        }
                    }
                    else
                    {
                        (*it)->setDead();
                    }
                    return true;
                }
            }
        }
        return false;
    }

    bool bacteriaOverlapsSocrates(double actorX, double actorY, int damage)
    {
        double socX = (m_player)->getX();
        double socY = (m_player)->getY();
        if (findEuclidean(actorX, actorY, socX, socY) <= (2 * SPRITE_RADIUS))
        {
            m_player->decHitPoints(damage);
            if (!m_player->isAlive())
            {
                m_player->setDead();
            }
            return true;
        }
        return false;
    }

    bool bacteriaOverlapsFood(double bacX, double bacY)
    {
        std::list<Actor*>::iterator it = actors.begin();
        for (; it != actors.end(); it++)
        {
            double x = (*it)->getX();
            double y = (*it)->getY();
            if ((*it)->isEdible())
            {
                if (findEuclidean(x, y, bacX, bacY) <= (2 * SPRITE_RADIUS))
                {
                    (*it)->setDead();
                    return true;
                }
            }
        }
        return false;
    }

    void findClosestFood(double bacX, double bacY, double& foodX, double& foodY)
    {
        double max = 0.0;
        std::list<Actor*>::iterator it = actors.begin();
        for (; it != actors.end(); it++)
        {
            double x = (*it)->getX();
            double y = (*it)->getY();
            double current = (findEuclidean(x, y, bacX, bacY));
            if (current > max)
            {
                max = current;
                foodX = x;
                foodY = y;
            }
        }
    }

    int findSocrates(double bacX, double bacY, double& socX, double& socY)
    {
        socX = (m_player)->getX();
        socY = (m_player)->getY();
        return findEuclidean(bacX, bacY, socX, socY);
    }

    void createGoodies();

private:
    Socrates* m_player;
    std::list<Actor*> actors;

    struct Coordinate
    {
        double x, y;
        Coordinate(double X, double Y) :x(X), y(Y) {}
    };
    std::vector<Coordinate> coords;
};



#endif // STUDENTWORLD_H_


