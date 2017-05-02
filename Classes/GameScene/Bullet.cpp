#include "Bullet.h"
#include "Enemy.h"
#include "GameManager.h"
#include "GameSettings.h"

Bullet* Bullet::create(BULLET_TYPE type)
{
	Bullet* bullet = new (std::nothrow) Bullet();
	if (bullet && bullet->init(type))
	{
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}

bool Bullet::init(BULLET_TYPE type)
{
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	switch (type)
	{
		case Bullet::PLAYER_BULLET_TYPE:
		{
			if (initWithFile("PlayerBullet.png"))
			{
				_type = type;
				_damage = 1;
				_speed = random(400, 600);
				_extraDamage - false;
				_sniperShot = false;
				this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
				this->setPhysicsBody(PhysicsBody::createCircle(this->getContentSize().width / 2, PhysicsMaterial(0, 1, 0)));
				this->getPhysicsBody()->setCollisionBitmask(GameManager::PLAYER_BULLET_BITMASK);
				this->getPhysicsBody()->setVelocityLimit(600);
				this->getPhysicsBody()->setAngularVelocityLimit(0);
				this->getPhysicsBody()->setContactTestBitmask(true);
				return true;
			}
		}
		case Bullet::ENEMY_BULLET_FIRST_TYPE:
		{
			if (initWithFile("FirstEnemyBullet.png"))
			{
				_type = type;
				_damage = random(5, 15);
				_speed = random(100, 150);
				_extraDamage - false;
				_sniperShot = false;
				this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
				this->setPhysicsBody(PhysicsBody::createCircle(this->getContentSize().width / 2, PhysicsMaterial(0, 0, 0)));
				this->getPhysicsBody()->addMass(0.05);
				this->getPhysicsBody()->setCollisionBitmask(GameManager::ENEMY_BULLET_BITMASK);
				this->getPhysicsBody()->setContactTestBitmask(true);
				return true;
			}
		}
		case Bullet::ENEMY_BULLET_SECOND_TYPE:
		{
			if (initWithFile("SecondEnemyBullet.png"))
			{
				_type = type;
				_damage = random(5, 15);
				_speed = 150;
				_extraDamage - false;
				_sniperShot = false;
				this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
				this->setPhysicsBody(PhysicsBody::createCircle(this->getContentSize().width / 2, PhysicsMaterial(0, 0, 0)));
				this->getPhysicsBody()->addMass(0.05);
				this->getPhysicsBody()->setCollisionBitmask(GameManager::ENEMY_BULLET_BITMASK);
				this->getPhysicsBody()->setContactTestBitmask(true);
				return true;
			}
		}
		default:
			return false;
	}
}

void Bullet::setSpeed(int speed)
{
	_speed = speed;
}

int Bullet::getSpeed()
{
	return _speed;
}

void Bullet::setDamage(int d)
{
	_damage = d;
}

int Bullet::getDamage()
{
	return _damage;
}

Bullet::BULLET_TYPE Bullet::getType()
{
	return _type;
}