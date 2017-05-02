#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include "Bullet.h"
#include "Bonus.h"

USING_NS_CC;

class Player : public Sprite
{
public:
	static Player* creat(int health, int shield);
	bool init(int health, int shield);

	void setBulletToShoot(Bullet* bullet);
	Bullet* getBulletToShoot();

	// player's gets hit
	void hit(int hit);

	int getShield() { return _shield; }
	int getHealth() { return _health; }
	bool isHaveShield() { return _haveShield; }
	void setHaveShiled(bool have) { _haveShield = have; }
	void setShield(int shield) { _shield = shield; }
	void setHealth(int health) { _health = health; }

	void setBonus(Bonus::BONUS_TYPE type);
	Bonus::BONUS_TYPE getBonus() { return _bonus; }

private:
	Sprite* _stick;

	int _health;
	int _shield;

	bool _haveShield;

	Bonus::BONUS_TYPE _bonus;

	// the bullet of the player, captured for "sniper bonus" or the beginning of the level
	Bullet* _bulletToShot;

	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);

};

#endif // !_PLAYER_H_
