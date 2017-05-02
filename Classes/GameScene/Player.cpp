#include "Player.h"
#include "GameManager.h"
#include"GameScene.h"
#include "GameSettings.h"

Player* Player::creat(int health, int shield)
{
	Player* player = new (std::nothrow) Player();
	if (player && player->init(health, shield))
	{
		player->autorelease();
		return player;
	}
	CC_SAFE_DELETE(player);
	return nullptr;
}

bool Player::init(int health, int shield)
{
	if (initWithFile("PlayerNormal.png"))
	{
		float scaleWidth = GameSettings::getInstance()->getWidthScale();
		float scaleHeight = GameSettings::getInstance()->getHeightScale();

		this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
		this->setPhysicsBody(PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0, 1, 0)));
		this->getPhysicsBody()->setCollisionBitmask(GameManager::PLAYER_BITMASK);
		this->getPhysicsBody()->setContactTestBitmask(true);
		this->getPhysicsBody()->setDynamic(false);

		_stick = Sprite::create("PlayerStickNormal.png");
		_stick->setContentSize(Size(_stick->getContentSize().width * scaleWidth, _stick->getContentSize().height * scaleHeight));
		_stick->setPosition(this->getContentSize().width / 2, -3 - _stick->getContentSize().height / 2);

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(Player::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(Player::onTouchMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(Player::onTouchEnded, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		_health = health;
		_shield = shield;

		if (_shield == 0)
		{
			this->setTexture("PlayerWhithoutShield.png");
			this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
			_haveShield = false;
			auto recovery = CallFuncN::create([](Node* node) {
				auto player = dynamic_cast<Player*>(node);
				auto scene = dynamic_cast<GameManager*>(node->getParent())->getGameScene();

				if (player->getShield() == 0)
				{
					float scaleWidth = GameSettings::getInstance()->getWidthScale();
					float scaleHeight = GameSettings::getInstance()->getHeightScale();
					player->setHaveShiled(true);
					player->setShield(10);
					scene->changeShield(10);
					player->setTexture("PlayerNormal.png");
					player->setContentSize(Size(player->getContentSize().width * scaleWidth, player->getContentSize().height * scaleHeight));
				}
			});
			auto delay = DelayTime::create(10);
			this->runAction(Sequence::create(delay, recovery, nullptr));
		}
		else
			_haveShield = true;

		_bonus = Bonus::NO_BONUS;

		this->addChild(_stick);

		return true;
	}

	return false;
}

bool Player::onTouchBegan(Touch* touch, Event* event)
{
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	Point managerPosition = Director::getInstance()->getRunningScene()->getChildByName("manager")->getPosition();
	Size managerSize = Director::getInstance()->getRunningScene()->getChildByName("manager")->getContentSize();
	Point location = touch->getLocation() - managerPosition;
	Point touchZonePos(this->getPosition().x - this->getContentSize().width/2,this->getPosition().y - this->getContentSize().height/2 - _stick->getContentSize().height - 3);
	Rect touchZoneRect(touchZonePos.x, touchZonePos.y, this->getContentSize().width, this->getContentSize().height + _stick->getContentSize().height + 3);
	Rect gameField(managerPosition.x - managerSize.width / 2, managerPosition.y - managerSize.height / 2, managerSize.width, managerSize.height);
	
	// checks to be whether the point of touch in the playing field
	if (!gameField.containsPoint(touch->getLocation()))
		return false;

	// checks to be the touch point in the zone of the player
	if (touchZoneRect.containsPoint(location))
	{
		_stick->setTexture("PlayerStickSelect.png");
		_stick->setContentSize(Size(_stick->getContentSize().width * scaleWidth, _stick->getContentSize().height * scaleHeight));

		if (_bulletToShot != nullptr)
			_bulletToShot->runAction(MoveTo::create(0.1, Vec2(location.x, _bulletToShot->getPosition().y)));

		this->runAction(MoveTo::create(0.1, Vec2(location.x, this->getPosition().y)));
		return true;
	}

	// check capture bullet
	if (_bulletToShot != nullptr)
	{
		if (this->_bonus == Bonus::SNIPER_SHOT)
			_bonus = Bonus::NO_BONUS;

		if (location.y <= (this->getPosition().y + this->getContentSize().height / 2))
			return false;

		Point pos = location - this->getPosition();
		float angele = pos.x>0?atan(pos.y / abs(pos.x)): M_PI - atan(pos.y / abs(pos.x));
		_bulletToShot->getPhysicsBody()->setVelocity(Vec2(_bulletToShot->getSpeed() * cos(angele), _bulletToShot->getSpeed() * sin(angele)));
		_bulletToShot->getPhysicsBody()->setCollisionBitmask(GameManager::PLAYER_BULLET_BITMASK);
		_bulletToShot = nullptr;

		return false;
	}

	return false;
}

void Player::onTouchMoved(Touch* touch, Event* event)
{
	Point location = touch->getLocation() - Director::getInstance()->getRunningScene()->getChildByName("manager")->getPosition();
	Size size(Director::getInstance()->getRunningScene()->getChildByName("manager")->getContentSize());
	Point rectPos(this->getPosition().x - _stick->getContentSize().width / 2, this->getPosition().y - 3 - _stick->getContentSize().height);
	Rect rect(rectPos, _stick->getContentSize());

	location.x = location.x >= size.width / 2 - this->getContentSize().width / 2 ? size.width / 2 - this->getContentSize().width / 2 : location.x;
	location.x = location.x <= -size.width / 2 + this->getContentSize().width / 2 ? -size.width / 2 + this->getContentSize().width / 2 : location.x;
	location.y = this->getPosition().y;

	this->setPosition(location);

	if (_bulletToShot != nullptr)
		_bulletToShot->runAction(Place::create(Point(location.x, _bulletToShot->getPosition().y)));
}

void Player::onTouchEnded(Touch* touch, Event* event)
{
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	_stick->setTexture("PlayerStickNormal.png");
	_stick->setContentSize(Size(_stick->getContentSize().width * scaleWidth, _stick->getContentSize().height * scaleHeight));
}

void Player::setBulletToShoot(Bullet* bullet)
{
	_bulletToShot = bullet;
}

Bullet* Player::getBulletToShoot()
{
	return _bulletToShot;
}

void Player::hit(int hit)
{
	auto scene = dynamic_cast<GameManager*>(this->getParent())->getGameScene();
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	_shield -= hit;
	
	if (_shield <= 0 && _haveShield)
	{
		_haveShield = false;

		this->setTexture("PlayerWhithoutShield.png");
		this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));

		hit = _shield + hit;
		_shield = 0;

		auto recovery = CallFuncN::create([](Node* node) {
			auto player = dynamic_cast<Player*>(node);
			auto scene = dynamic_cast<GameManager*>(node->getParent())->getGameScene();

			if (player->getShield() == 0)
			{
				float scaleWidth = GameSettings::getInstance()->getWidthScale();
				float scaleHeight = GameSettings::getInstance()->getHeightScale();
				player->setHaveShiled(true);
				player->setShield(10);
				scene->changeShield(10);
				player->setTexture("PlayerNormal.png");
				player->setContentSize(Size(player->getContentSize().width * scaleWidth, player->getContentSize().height * scaleHeight));
			}
		});

		auto delay = DelayTime::create(10);

		scene->changeShield(-hit);

		this->runAction(Sequence::create(delay, recovery, nullptr));

		return;
	}

	if (_shield < 0 && !_haveShield)
	{
		_health--;
		_shield = 0;

		if (_health == 0)
			dynamic_cast<GameManager*>(this->getParent())->restartGame();

		scene->changeHealth(-1);
		return;
	}

	scene->changeShield(-hit);
}

void Player::setBonus(Bonus::BONUS_TYPE type)
{
	switch (type)
	{
	case Bonus::NO_BONUS:
		_bonus = type;
		break;
	case Bonus::SNIPER_SHOT:
		_bonus = type;
		break;
	case Bonus::EXTRA_DAMAGE:
		_bonus = type;
		break;
	case Bonus::RECOVERY_SHIELD:
	{
		auto scene = dynamic_cast<GameManager*>(this->getParent())->getGameScene();
		int inc = random(10, 15);
		float scaleWidth = GameSettings::getInstance()->getWidthScale();
		float scaleHeight = GameSettings::getInstance()->getHeightScale();

		inc = _shield + inc > 100 ? 100 - _shield : inc;

		if (_shield == 0)
		{
			this->stopAllActions();
			_haveShield = true;
			this->setTexture("PlayerNormal.png");
			this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
		}

		_shield += inc;
		scene->changeShield(inc);

		break;
	}
	case Bonus::RECOVERY_HEALTH:
	{
		auto scene = dynamic_cast<GameManager*>(this->getParent())->getGameScene();

		_health++;

		scene->changeHealth(1);

		break;
	}
	default:
		break;
	}
}