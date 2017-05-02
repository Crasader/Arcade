#include "Bonus.h"
#include "GameManager.h"
#include "GameSettings.h"

Bonus* Bonus::create(BONUS_TYPE type)
{
	Bonus* bonus = new (std::nothrow) Bonus();
	if (bonus && bonus->init(type))
	{
		bonus->autorelease();
		return bonus;
	}
	CC_SAFE_DELETE(bonus);
	return nullptr;
}

bool Bonus::init(BONUS_TYPE type)
{
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	switch (type)
	{
	case Bonus::SNIPER_SHOT:
		if (initWithFile("BonusSniperShot.png"))
		{
			_type = type;
			_speed = 150;
			this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
			this->setPhysicsBody(PhysicsBody::createCircle(this->getContentSize().width / 2, PhysicsMaterial(0, 0, 0)));
			this->getPhysicsBody()->setCollisionBitmask(GameManager::BONUS_BITMASK);
			this->getPhysicsBody()->setContactTestBitmask(true);
			return true;
		}
	case Bonus::EXTRA_DAMAGE:
		if (initWithFile("BonusExtraDamage.png"))
		{
			_type = type;
			_speed = 150;
			this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
			this->setPhysicsBody(PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0, 0, 0)));
			this->getPhysicsBody()->setCollisionBitmask(GameManager::BONUS_BITMASK);
			this->getPhysicsBody()->setContactTestBitmask(true);
			return true;
		}
	case Bonus::RECOVERY_SHIELD:
		if (initWithFile("BonusRecoveryShield.png"))
		{
			_type = type;
			_speed = 150;
			this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
			this->setPhysicsBody(PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0, 0, 0)));
			this->getPhysicsBody()->setCollisionBitmask(GameManager::BONUS_BITMASK);
			this->getPhysicsBody()->setContactTestBitmask(true);
			return true;
		}
	case Bonus::RECOVERY_HEALTH:
		if (initWithFile("BonusHealth.png"))
		{
			_type = type;
			_speed = 150;
			this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
			this->setPhysicsBody(PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0, 0, 0)));
			this->getPhysicsBody()->setCollisionBitmask(GameManager::BONUS_BITMASK);
			this->getPhysicsBody()->setContactTestBitmask(true);
			return true;
		}
	default:
		return false;
	}
}