#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED

#include "GraphObject.h"

class StudentWorld;
class Socrates;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
	Actor(int imID, double startX, double startY, StudentWorld* swptr, Direction dir=0, int depth=0);
	virtual void doSomething();

	StudentWorld* getWorld() const;

	bool isAlive();
	void setDead();

	virtual bool isDamagable() { return false; }
	virtual bool isLiveDamagable() { return false; }
	virtual void decHitPoints(int damage) {}
	virtual bool takeDamage(int damage);
	virtual bool blocksBacteria() const;
	virtual bool isEdible() const;

	virtual ~Actor() { delete sw; }

private:
	StudentWorld* sw;
	bool m_alive;
};


class Dirt :public Actor
{
public:
	Dirt(double startX, double startY, StudentWorld* swptr);
	//virtual void doSomething();
	virtual bool isDamagable() { return true; }
	virtual bool takeDamage(int) { setDead(); return true; }
	virtual bool blocksBacteria() const {return true;}
};

class Food : public Actor
{
public:
	Food(double startX, double startY, StudentWorld* swptr);
	//virtual void doSomething();
	virtual bool isEdible() const { return true; }
};

class Pit : public Actor
{
public: 
	Pit(double startX, double startY, StudentWorld* swptr);
	virtual void doSomething();
	//virtual bool preventsLevelCompleting() const;
private:
	int m_regSalmonella;
	int m_aggSalmonella;
	int m_eColi;
};

class Projectile :public Actor
{
public:
	Projectile(const int id, double startX, double startY, StudentWorld* swptr, Direction d, int maxLife);
	int getMaxLife() { return m_maxLife; }
	bool isMax() { return m_maxLife <= 0; }
	virtual void doSomething();
private:
	int m_maxLife;
};

class Spray : public Projectile
{
public:
	Spray(double startX, double startY, StudentWorld* swptr, Direction d);
	virtual void doSomething();
};

class Flame: public Projectile
{
public:
	Flame(double startX, double startY, StudentWorld* swptr, Direction d);
	void doSomething();

};


class Goodie : public Actor
{
public:
	Goodie(const int imageID, double x, double y, StudentWorld* swptr);
	virtual bool isDamagable() { return true; }
	virtual bool takeDamage(int) { setDead(); return true; }
	void doSomething();

	// Have s pick up this goodie.
	//virtual void pickUp(Socrates* s) = 0;
private:
	int m_maxLife;
};

class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(double x, double y, StudentWorld* swptr);
	virtual void doSomething();
	//virtual void pickUp(Socrates* s);
};

class FlameThrowerGoodie : public Goodie
{
public:
	FlameThrowerGoodie(double x, double y, StudentWorld* swptr);
	virtual void doSomething();
	//virtual void pickUp(Socrates* s);
};

class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(double x, double y, StudentWorld* swptr);
	virtual void doSomething();
	//virtual void pickUp(Socrates* s);
};

class Fungus : public Goodie
{
public:
	Fungus(double x, double y, StudentWorld* swptr);
	virtual void doSomething();
	//virtual void pickUp(Socrates* s);
};



class Agent : public Actor
{
public:
	Agent(const int id, double startX, double startY, StudentWorld* swptr, int hp, Direction dir = 0, int depth = 0, double size = 1.0);
	virtual bool isDamagable() { return true; }
	virtual bool isLiveDamagable() { return true; }
	void decHitPoints(int damage) { m_hp -= damage; }
	virtual bool takeDamage(int damage); 
	//void doSomething();


	int getHitPoints() const;
	void restoreHealth() { m_hp = 100; }

private:
	int m_hp;
};

class Socrates : public Agent
{
public: 
	Socrates(double startX, double startY, StudentWorld* swptr);
	void Socrates::moveSocrates(double d);
	void doSomething();

	int Socrates::getSprays() const { return m_sprayCharges; }
	int Socrates::getFlames() const { return m_flameCharges; }

	void restoreFlame() { m_flameCharges += 5; }

private:
	int m_sprayCharges;
	int m_flameCharges;
};

class Bacteria : public Agent
{
public:
	Bacteria(const int id, double startX, double startY, StudentWorld* swptr, int hp);
};

class Salmonella : public Bacteria
{
public:
	Salmonella(const int id, double startX, double startY, StudentWorld* swptr, int hp);
	int getMPD() { return mpd; }
	void changeMPD(int c) { mpd += c; }
	void resetMPD() { mpd = 10; }

	int getFood() { return m_food; }
	void changeFood(int c) { m_food += c; }
	void resetFood() { m_food = 0; }
	//virtual void doSomething();
private:
	int mpd;
	int m_food;
};

class RegularSalmonella : public Salmonella
{
public:
	RegularSalmonella(double startX, double startY, StudentWorld* swptr);
	void doSomething();
};

class AggressiveSalmonella : public Salmonella
{
public:
	AggressiveSalmonella(double startX, double startY, StudentWorld* swptr);
};

class eColi :public Bacteria
{
public:
	eColi(double startX, double startY, StudentWorld* swptr);
};
#endif // ACTOR_H_
