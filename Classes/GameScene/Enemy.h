#ifndef _ENEMY_H_ 
#define _ENEMY_H_

#include "cocos2d.h"
#include "Bullet.h"

USING_NS_CC;

class Enemy : public Sprite
{
public:
	enum ENEMY_TYPE
	{
		EMPTY_TYPE,
		FIRST_TYPE,
		SECOND_TYPE,
		THIRD_TYPE,
		FOURTH_TYPE
	};

	static Enemy* create(ENEMY_TYPE type, int pos, bool res);

	//set enemy formation
	static void setFormation(std::vector<int> form);
	static void setEnemyInFormation(ENEMY_TYPE type, int pos);

	bool init(ENEMY_TYPE type, int pos, bool res);

	// delete enemy from game
	void removeFromManager();

	// player's bullet
	void setCaptureBullet(Bullet* bullet);
	Bullet* getCaptureBullet();

	ENEMY_TYPE getType() { return _type; }

	// kill enemy
	void dead();

	// enemy gets hit
	void hit(int hit);

private:
	// enemy formation
	static int _formation[6][6];

	int _health;
	int _damage;
	int _score;

	int _bonusChance;

	ENEMY_TYPE _type;

	// enemy position in formation
	int _positionFormation;

	// player's bullet captured
	Bullet* _capture;

	// count of revived enemies
	static int _respawnCount;
	bool _isRespawn;

	// attacks of enemies
	void firstAttack(float d);
	void secondAttack(float d);
	void thirdAttack(float d);
	void fourthAttack(float d);
};

#endif // !_ENEMY_H_ 

