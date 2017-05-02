#ifndef _BONUS_H_
#define _BONUS_H_

#include "cocos2d.h"

USING_NS_CC;

class Bonus : public Sprite
{
public:

	enum BONUS_TYPE
	{
		NO_BONUS,
		SNIPER_SHOT,
		EXTRA_DAMAGE,
		RECOVERY_SHIELD,
		RECOVERY_HEALTH
	};

	static Bonus* create(BONUS_TYPE type);
	bool init(BONUS_TYPE type);

	BONUS_TYPE getType() { return _type; }

	int getSpeed() { return _speed; }
	void setSpeed(int speed) { _speed = speed; }

private:
	BONUS_TYPE _type;

	int _speed;
};

#endif // !_BONUS_H_