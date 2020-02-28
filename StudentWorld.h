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
    bool inBound(double x, double y);

    // Add an actor to the world.
    void addActor(Actor* a);

    // If actor a ovelaps some live actor, damage that live actor by the
    // indicated amount of damage and return true; otherwise, return false.
    bool damageOneActor(Actor* a, int damage);

    // Is bacterium a blocked from moving to the indicated location?
    bool isBacteriumMovementBlockedAt(double x, double y) const;

    // If actor a overlaps this world's socrates, return a pointer to the
    // socrates; otherwise, return nullptr
    Socrates* getOverlappingSocrates(Actor* a) const;


    // Return true if this world's socrates is within the indicated distance
    // of actor a; otherwise false.  If true, angle will be set to the
    // direction from actor a to the socrates.
    bool getAngleToNearbySocrates(Actor* a, int dist, int& angle) const;

    bool pitExists(int i);

    bool overlapsProjectile(double projX, double projY, int damage);

    bool bacteriaOverlapsSocrates(double actorX, double actorY, int damage);

    bool bacteriaOverlapsFood(double bacX, double bacY);

    bool findClosestFood(double bacX, double bacY, double& foodX, double& foodY, double dist);

    bool findSocrates(double bacX, double bacY, double& socX, double& socY, double dist);

    void createGoodies();
    void statsText();

private:
    Socrates* m_player;
    std::list<Actor*> actors;

    struct Coordinate
    {
        double x, y;
        Coordinate(double X, double Y) :x(X), y(Y) {}
    };
    std::vector<Coordinate> coords;
    bool pit;
};



#endif // STUDENTWORLD_H_


