#include "Enemy.h"
#include "GameManager.h"
#include "Bullet.h"
#include "GameScene.h"
#include "Bonus.h"
#include "SimpleAudioEngine.h"
#include "GameSettings.h"

using namespace CocosDenshion;

int Enemy::_formation[6][6] = { {0, 0, 0, 0, 0, 0},
							    {0, 0, 0, 0, 0, 0,},
						        {0, 0, 0, 0, 0, 0,},
						        {0, 0, 0, 0, 0, 0,},
						        {0, 0, 0, 0, 0, 0,},
						        {0, 0, 0, 0, 0, 0,}
};

int Enemy::_respawnCount = 0;

Enemy* Enemy::create(Enemy::ENEMY_TYPE type, int pos, bool res)
{
	Enemy* enemy = new (std::nothrow) Enemy();
	if (enemy && enemy->init(type, pos, res))
	{
		enemy->autorelease();
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return nullptr;
}

bool Enemy::init(Enemy::ENEMY_TYPE type, int pos, bool res)
{
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	switch (type)
	{
		case Enemy::FIRST_TYPE:
		{
			if (initWithFile("FirstEnemy.png"))
			{
				_type = type;
				_health = 1;
				_damage = 0;
				_score = 1;
				_positionFormation = pos;
				_capture = nullptr;
				_isRespawn = res;
				_bonusChance = 5;
				this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
				this->setPhysicsBody(PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0, 1, 0)));
				this->getPhysicsBody()->setDynamic(false);
				this->getPhysicsBody()->setCollisionBitmask(GameManager::ENEMY_BITMASK);
				this->getPhysicsBody()->setContactTestBitmask(true);
				schedule(CC_SCHEDULE_SELECTOR(Enemy::firstAttack), random(10,15), CC_REPEAT_FOREVER, 0);
				return true;
			}
		}
		case Enemy::SECOND_TYPE:
		{
			if (initWithFile("SecondEnemy.png"))
			{
				_type = type;
				_health = 2;
				_damage = 1;
				_score = 2;
				_positionFormation = pos;
				_capture = nullptr;
				_isRespawn = res;
				_bonusChance = 5;
				this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
				this->setPhysicsBody(PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0, 1, 0)));
				this->getPhysicsBody()->setDynamic(false);
				this->getPhysicsBody()->setCollisionBitmask(GameManager::ENEMY_BITMASK);
				this->getPhysicsBody()->setContactTestBitmask(true);
				schedule(CC_SCHEDULE_SELECTOR(Enemy::secondAttack), random(15,20), CC_REPEAT_FOREVER, 0);
				return true;
			}
		}
		case Enemy::THIRD_TYPE:
		{
			if (initWithFile("ThirdEnemy.png"))
			{
				_type = type;
				_health = 1;
				_damage = 0;
				_score = 4;
				_positionFormation = pos;
				_capture = nullptr;
				_isRespawn = res;
				_bonusChance = 10;
				this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
				this->setPhysicsBody(PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0, 1, 0)));
				this->getPhysicsBody()->setDynamic(false);
				this->getPhysicsBody()->setCollisionBitmask(GameManager::ENEMY_BITMASK);
				this->getPhysicsBody()->setContactTestBitmask(true);
				schedule(CC_SCHEDULE_SELECTOR(Enemy::thirdAttack), random(25, 50), CC_REPEAT_FOREVER, 0);
				return true;
			}
		}
		case Enemy::FOURTH_TYPE:
		{
			if (initWithFile("FourthEnemy.png"))
			{
				_type = type;
				_health = 3;
				_damage = 0;
				_score = 5;
				_positionFormation = pos;
				_capture = nullptr;
				_isRespawn = res;
				_bonusChance = 20;
				this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));
				this->setPhysicsBody(PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0, 1, 0)));
				this->getPhysicsBody()->setDynamic(false);
				this->getPhysicsBody()->setCollisionBitmask(GameManager::ENEMY_BITMASK);
				this->getPhysicsBody()->setContactTestBitmask(true);
				schedule(CC_SCHEDULE_SELECTOR(Enemy::fourthAttack), random(30,50), CC_REPEAT_FOREVER, 0);
				return true;
			}
		}
		default:
			return false;
	}
}

void Enemy::setFormation(std::vector<int> form)
{
	for (int i = 0; i < form.size(); i++)
		_formation[i / 6][i % 6] = form.at(i);
}

void Enemy::setEnemyInFormation(ENEMY_TYPE type, int pos)
{
	_formation[pos / 6][pos % 6] = type;
}

void Enemy::removeFromManager()
{
	auto manager = dynamic_cast<GameManager*>(this->getParent());
	manager->removeEnemy(_positionFormation);
}

void Enemy::dead()
{
	this->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
	auto manager = dynamic_cast<GameManager*>(this->getParent());

	int pos = _positionFormation;
	_formation[pos / 6][pos % 6] = false;

	if (_isRespawn)
		_respawnCount--;

	// splash damage
	if (_damage != 0)
	{
		Rect damage(this->getPosition().x - 1.5*this->getContentSize().width,
					this->getPosition().y - 1.5*this->getContentSize().height,
					3 * this->getContentSize().width,
					3 * this->getContentSize().height);

		int i = pos / 6 - 1;
		int j = pos % 6 - 1;

		if ((i >= 0) && (i <= 5) && (j >= 0) && (j <= 5) && _formation[i][j] && damage.containsPoint(manager->getEnemy(i * 6 + j)->getPosition()))
				manager->getEnemy(i * 6 + j)->hit(_damage);

		i = pos / 6 - 1;
		j = pos % 6;

		if ((i >= 0) && (i <= 5) && (j >= 0) && (j <= 5) && _formation[i][j] && damage.containsPoint(manager->getEnemy(i * 6 + j)->getPosition()))
			manager->getEnemy(i * 6 + j)->hit(_damage);

		i = pos / 6 - 1;
		j = pos % 6 + 1;

		if ((i >= 0) && (i <= 5) && (j >= 0) && (j <= 5) && _formation[i][j] && damage.containsPoint(manager->getEnemy(i * 6 + j)->getPosition()))
			manager->getEnemy(i * 6 + j)->hit(_damage);

		i = pos / 6;
		j = pos % 6 - 1;

		if ((i >= 0) && (i <= 5) && (j >= 0) && (j <= 5) && _formation[i][j] && damage.containsPoint(manager->getEnemy(i * 6 + j)->getPosition()))
			manager->getEnemy(i * 6 + j)->hit(_damage);

		i = pos / 6 + 1;
		j = pos % 6 - 1;

		if ((i >= 0) && (i <= 5) && (j >= 0) && (j <= 5) && _formation[i][j] && damage.containsPoint(manager->getEnemy(i * 6 + j)->getPosition()))
			manager->getEnemy(i * 6 + j)->hit(_damage);

		i = pos / 6 + 1;
		j = pos % 6;

		if ((i >= 0) && (i <= 5) && (j >= 0) && (j <= 5) && _formation[i][j] && damage.containsPoint(manager->getEnemy(i * 6 + j)->getPosition()))
			manager->getEnemy(i * 6 + j)->hit(_damage);

		i = pos / 6 + 1;
		j = pos % 6 + 1;

		if ((i >= 0) && (i <= 5) && (j >= 0) && (j <= 5) && _formation[i][j] && damage.containsPoint(manager->getEnemy(i * 6 + j)->getPosition()))
			manager->getEnemy(i * 6 + j)->hit(_damage);

		i = pos / 6;
		j = pos % 6 + 1;

		if ((i >= 0) && (i <= 5) && (j >= 0) && (j <= 5) && _formation[i][j] && damage.containsPoint(manager->getEnemy(i * 6 + j)->getPosition()))
			manager->getEnemy(i * 6 + j)->hit(_damage);
	}

	if (_bonusChance/random(1,100))
	{
		auto bonus = CallFuncN::create([](Node* node) {
			Bonus* bonus;
			auto enemy = dynamic_cast<Enemy*>(node);
			auto manager = dynamic_cast<GameManager*>(enemy->getParent());

			if(enemy->getType()==Enemy::FOURTH_TYPE)
				bonus = Bonus::create(Bonus::BONUS_TYPE(random(1, 4)));
			else
				bonus = Bonus::create(Bonus::BONUS_TYPE(random(1, 3)));


			bonus->setPosition(enemy->getPosition().x, enemy->getPosition().y - enemy->getContentSize().height / 2 - bonus->getContentSize().height / 2 - 3);
			bonus->getPhysicsBody()->setVelocity(Vec2(0, -bonus->getSpeed()));
			manager->addChild(bonus);
		});
		this->runAction(bonus);
	}

	// death enemy
	auto kill = CallFuncN::create([](Node* node) {
		auto enemy = dynamic_cast<Enemy*>(node);
		auto manager = enemy->getParent();

		if (enemy->getCaptureBullet() != nullptr)
		{
			auto bullet = enemy->getCaptureBullet();
			bullet->getPhysicsBody()->setCollisionBitmask(GameManager::PLAYER_BULLET_BITMASK);
			float angele = MATH_DEG_TO_RAD(random(0, 359));
			bullet->getPhysicsBody()->setVelocity(Vec2(random(0, bullet->getSpeed())*cos(angele), random(0, bullet->getSpeed())*sin(angele)));
		}

		std::string texture;

		switch (enemy->getType())
		{
		case Enemy::EMPTY_TYPE:
			break;
		case Enemy::FIRST_TYPE:
			texture = "FirstEnemyFrag.png";
			break;
		case Enemy::SECOND_TYPE:
			texture = "SecondEnemyFrag.png";
			break;
		case Enemy::THIRD_TYPE:
			texture = "ThirdEnemyFrag.png";
			break;
		case Enemy::FOURTH_TYPE:
			texture = "FourthEnemyFrag.png";
			break;
		default:
			break;
		}

		// frags of enemy
		for (int i = 0; i < 3; i++)
		{
			auto frag = Sprite::create(texture);
			float scaleWidth = GameSettings::getInstance()->getWidthScale();
			float scaleHeight = GameSettings::getInstance()->getHeightScale();

			frag->setContentSize(Size(frag->getContentSize().width * scaleWidth, frag->getContentSize().height * scaleHeight));
			frag->setPhysicsBody(PhysicsBody::createBox(frag->getContentSize(), PhysicsMaterial(0, 1, 0)));
			frag->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
			frag->getPhysicsBody()->setContactTestBitmask(true);
			frag->setPosition(enemy->getPosition().x - frag->getContentSize().width + i*(frag->getContentSize().width + 3), enemy->getPosition().y);
			frag->getPhysicsBody()->setVelocity(Vec2(random(-50, 50), random(-50, 50)));
			
			auto clear = CallFuncN::create([](Node* node) {
				node->removeFromParent();
			});

			auto fadeOut = FadeOut::create(1);

			frag->runAction(Sequence::create(fadeOut, clear, nullptr));
			manager->addChild(frag);
		}

		enemy->removeFromManager();
	});

	auto scene = dynamic_cast<GameManager*>(this->getParent())->getGameScene();
	scene->changeScore(_score);
	
	this->runAction(kill);
}


void Enemy::hit(int hit)
{
	if (this == nullptr)
		return;

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	_health = (_health - hit) <= 0 ? 0 : _health - hit;
	switch (_health)
	{
		case 0:
			dead();
			break;
		case 1:
		{
			if (_type == SECOND_TYPE)
				this->setTexture("SecondEnemyDamage.png");

			if (_type == FOURTH_TYPE)
				this->setTexture("FourthEnemyDamage2.png");

			this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));

			break;
		}
		case 2:
		{
			if (_type == FOURTH_TYPE)
				this->setTexture("FourthEnemyDamage1.png");

			this->setContentSize(Size(this->getContentSize().width * scaleWidth, this->getContentSize().height * scaleHeight));

			break;
		}
		default:
			break;
	}		
}


void Enemy::firstAttack(float d)
{
	bool free = true;
	for (int i = _positionFormation / 6 + 1; i < 6; i++)
		free = free && !_formation[i][_positionFormation % 6];

	if (free)
	{
		Point attackPosition(this->getPosition().x, 2 * this->getContentSize().height);
		int speed = 100;
		float distation = this->getPosition().y - attackPosition.y;
		auto moveForvard = MoveTo::create(distation / speed, attackPosition);
		auto moveBack = MoveTo::create(distation / speed, this->getPosition());

		auto shoot = CallFuncN::create([](Node* node) {
			auto enemy = dynamic_cast<Enemy*>(node);
			auto manager = dynamic_cast<GameManager*>(enemy->getParent());
			auto bullet = Bullet::create(Bullet::ENEMY_BULLET_FIRST_TYPE);
			bullet->setPosition(enemy->getPosition().x, enemy->getPosition().y - enemy->getContentSize().height / 2 - bullet->getContentSize().height / 2);

			Point pos = bullet->getPosition() - manager->getPlayer()->getPosition();
			float angele = pos.x>0 ? M_PI + atan(pos.y / abs(pos.x)) : 2 * M_PI - atan(pos.y / abs(pos.x));
			bullet->getPhysicsBody()->setVelocity(Vec2(bullet->getSpeed() * cos(angele), bullet->getSpeed() * sin(angele)));

			manager->addChild(bullet);
			SimpleAudioEngine::getInstance()->playEffect("EnemyShot.wav", false, 1.0, 1.0, 1.0);
		});
		this->runAction(Sequence::create(moveForvard, shoot, moveBack, nullptr));
	}
}

void Enemy::secondAttack(float d)
{
	bool free = true;
	for (int i = _positionFormation / 6 + 1; i < 6; i++)
		free = free && !_formation[i][_positionFormation % 6];

	if (free)
	{
		auto shoot = CallFuncN::create([](Node* node) {
			auto enemy = dynamic_cast<Enemy*>(node);
			auto manager = dynamic_cast<GameManager*>(enemy->getParent());
			auto bullet = Bullet::create(Bullet::ENEMY_BULLET_SECOND_TYPE);
			bullet->setPosition(enemy->getPosition().x, enemy->getPosition().y - enemy->getContentSize().height / 2 - bullet->getContentSize().height / 2);
			bullet->getPhysicsBody()->setVelocity(Vec2(0, -bullet->getSpeed()));
			manager->addChild(bullet);
			SimpleAudioEngine::getInstance()->playEffect("EnemyShot.wav", false, 1.0, 1.0, 1.0);
		});
		Vector<FiniteTimeAction*> actions;
		auto delay = DelayTime::create(0.5);

		actions.pushBack(shoot);
		actions.pushBack(delay);
		int count = random(2, 4);
		for (int i = 0; i < count; i++)
		{
			actions.pushBack(shoot->clone());
			actions.pushBack(delay->clone());
		}
		this->runAction(Sequence::create(actions));
	}
}

void Enemy::thirdAttack(float d)
{
	auto field = Sprite::create("ThirdField.png");
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	field->setContentSize(Size(field->getContentSize().width * scaleWidth, field->getContentSize().height * scaleHeight));
	field->setPhysicsBody(PhysicsBody::createCircle(field->getContentSize().width / 2, PhysicsMaterial(0, 0, 0)));
	field->getPhysicsBody()->setCollisionBitmask(GameManager::ENEMY_FIELD_BITMASK);
	field->getPhysicsBody()->setContactTestBitmask(true);
	field->getPhysicsBody()->setDynamic(false);

	field->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(field);

	auto remove = CallFuncN::create([](Node* node) {
		node->removeFromParent();
	});
	auto delay = DelayTime::create(5);
	field->runAction(Sequence::create(delay, remove, nullptr));

}

void Enemy::fourthAttack(float d)
{
	auto manager = dynamic_cast<GameManager*>(this->getParent());
	if (_respawnCount < 4)
	{
		_respawnCount++;
		manager->respawnEnemy(ENEMY_TYPE(random(1, 2)));
	}
}

void Enemy::setCaptureBullet(Bullet* bullet)
{
	_capture = bullet;
}

Bullet* Enemy::getCaptureBullet()
{
	return _capture;
}