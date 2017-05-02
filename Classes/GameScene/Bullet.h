#ifndef _BULLET_H_
#define _BULLET_H_

#include "cocos2d.h"

USING_NS_CC;

class Bullet : public Sprite
{
public:
	enum BULLET_TYPE
	{
		PLAYER_BULLET_TYPE,
		ENEMY_BULLET_FIRST_TYPE,
		ENEMY_BULLET_SECOND_TYPE
	};
	static Bullet* create(BULLET_TYPE type);
	bool init(BULLET_TYPE type);

	int getDamage();
	void setDamage(int d);

	BULLET_TYPE getType();

	void setSpeed(int speed);
	int getSpeed();

	bool getExtraDamage() { return _extraDamage; }
	void setExtraDamage(bool extra) { _extraDamage = extra; }

	bool getSniperShot() { return _sniperShot; }
	void setSniperShot(bool sniper) { _sniperShot = sniper; }

private:
	BULLET_TYPE _type;
	int _damage;
	bool _extraDamage;
	bool _sniperShot;
	int _speed;
};

#endif // !_BULLET_H_
