#include "GameManager.h"
#include "GameScene.h"
#include "ui\CocosGUI.h"
#include "Bonus.h"
#include "LevelCompleteScene.h"
#include "SimpleAudioEngine.h"
#include "GameSettings.h"

using namespace ui;
using namespace CocosDenshion;

GameManager* GameManager::create(GameScene* scene)
{
	GameManager * ret = new (std::nothrow) GameManager();
	if (ret && ret->init(scene))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool GameManager::init(GameScene* scene)
{
	_gameScene = scene;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	Size edgeBodySize(visibleSize.width, visibleSize.height - 70 * scaleHeight);
	auto edgeBody = PhysicsBody::createEdgeBox(edgeBodySize, PhysicsMaterial(0, 1, 0), 1);
	this->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 30 * scaleHeight));
	this->setPhysicsBody(edgeBody);
	this->getPhysicsBody()->setCollisionBitmask(EDGE_BITMASK);
	this->getPhysicsBody()->setContactTestBitmask(true);
	this->setContentSize(edgeBodySize);

	auto bottomEdge = Node::create();
	bottomEdge->setPhysicsBody(PhysicsBody::createBox(Size(edgeBodySize.width, 1), PhysicsMaterial(0, 1, 0)));
	bottomEdge->setPosition(0, 90 * scaleHeight - edgeBodySize.height / 2);
	bottomEdge->getPhysicsBody()->setDynamic(false);
	bottomEdge->getPhysicsBody()->setCollisionBitmask(GameManager::BOTTOM_EDGE_BITMASK);
	bottomEdge->getPhysicsBody()->setContactTestBitmask(true);
	this->addChild(bottomEdge);

	// phalanx ememy formation
	{
		std::vector<int> tmp = { 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1
		};
		_maskFormation.push_back(tmp);
	}


	// wedge ememy formation
	{
		std::vector<int> tmp = { 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1,
								 0, 1, 1, 1, 1, 0,
								 0, 0, 1, 1, 0, 0,
								 0, 0, 1, 1, 0, 0
		};
		_maskFormation.push_back(tmp);
	}


	// square ememy formation
	{
		std::vector<int> tmp = { 1, 1, 1, 1, 1, 1,
								 1, 0, 0, 0, 0, 1,
								 1, 0, 1, 1, 0, 1,
								 1, 0, 1, 1, 0, 1,
								 1, 0, 0, 0, 0, 1,
								 1, 1, 1, 1, 1, 1
		};
		_maskFormation.push_back(tmp);
	}

	// reverse wedge ememy formation
	{
		std::vector<int> tmp = { 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1,
								 1, 1, 1, 1, 1, 1,
								 1, 1, 0, 0, 1, 1,
								 1, 0, 0, 0, 0, 1,
								 1, 0, 0, 0, 0, 1
		};
		_maskFormation.push_back(tmp);
	}

	// random wedge ememy formation
	{
		std::vector<int> tmp = { random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
								 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
								 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
								 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
								 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
								 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1)
		};
		_maskFormation.push_back(tmp);
	}

	_enemyFormation = PHALANX_FORMATION;
	_isCustomEnemyFormation = false;
	_isCustomFormationWithEnemyType = false;

	// calculate enemy position
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
		{
			Size enemy(84 * scaleWidth, 44 * scaleHeight);
			_enemyPosition.push_back(enemyPosition{ nullptr, Point(10 * scaleWidth + j * (3 + enemy.width) + enemy.width / 2 - this->getContentSize().width / 2, this->getContentSize().height / 2 - enemy.height / 2 - i * (3 + enemy.height) - 3)});
			_freePosition.push_back(_enemyPosition.size() - 1);
		}

	return true;
}

void GameManager::setEnemyFormation(ENEMY_FORMATION form)
{
	_enemyFormation = form;
}

void GameManager::setCustomEnemyFormation(std::vector<int> form)
{
	_customEnemyFormation = form;
	_isCustomEnemyFormation = true;
	_isCustomFormationWithEnemyType = false;
}

void GameManager::setCustomFormationWhithEnemyType(std::vector<int> form)
{
	_customFormationWithEnemyType = form;
	_isCustomEnemyFormation = false;
	_isCustomFormationWithEnemyType = true;
}

void GameManager::setEnemyInPosition(Enemy::ENEMY_TYPE type, int pos, bool res)
{
	if (_enemyPosition.at(pos).enemy != nullptr)
	{
		_enemyPosition.at(pos).enemy->removeFromParent();
		_enemyPosition.at(pos).enemy = nullptr;
	}

	auto enemy = Enemy::create(type, pos, res);
	enemy->setPosition(_enemyPosition.at(pos).position);
	_enemyPosition.at(pos).enemy = enemy;
	
	int free;
	for (free = 0; free < _freePosition.size(); free++)
		if (_freePosition.at(free) == pos)
			break;

	std::vector<int>::iterator it = _freePosition.begin();

	if (free != _freePosition.size())
		_freePosition.erase(it+free);

	Enemy::setEnemyInFormation(type, pos);
	this->addChild(enemy);
}

void GameManager::newRandomFormation()
{
	std::vector<int> tmp = { random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
							 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
							 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
							 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
							 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1),
							 random(0,1), random(0,1), random(0,1), random(0,1), random(0,1), random(0,1)
	};
	_maskFormation.at(RANDOM_FORMATION) = tmp;
}

void GameManager::runGame()
{
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameManager::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	// create custom enemy formation with set type
	if (_isCustomFormationWithEnemyType)
	{
		for (int i = 0; i < _customFormationWithEnemyType.size(); i++)
			switch (_customFormationWithEnemyType.at(i))
			{
				case 0:
					_currentFormation.push_back(0);
					break;
				case 1:
				{
					setEnemyInPosition(Enemy::FIRST_TYPE, i, false);
					_currentFormation.push_back(Enemy::FIRST_TYPE);
					break;
				}
				case 2:
				{
					setEnemyInPosition(Enemy::SECOND_TYPE, i, false);
					_currentFormation.push_back(Enemy::SECOND_TYPE);
					break;
				}
				case 3:
				{
					setEnemyInPosition(Enemy::THIRD_TYPE, i, false);
					_currentFormation.push_back(Enemy::THIRD_TYPE);
					break;
				}
				case 4:
				{
					setEnemyInPosition(Enemy::FOURTH_TYPE, i, false);
					_currentFormation.push_back(Enemy::FOURTH_TYPE);
					break;
				}
				default:
					break;
			}
		addPlayer();
		_gameScene->setFormation(_currentFormation);
		return;
	}

	// create custom enemy formation with random type
	if (_isCustomEnemyFormation)
	{
		for (int i = 0; i < _customEnemyFormation.size(); i++)
		{
			if (_customEnemyFormation.at(i))
			{
				int type = random(1, 4);
				_currentFormation.push_back(type);
				setEnemyInPosition(Enemy::ENEMY_TYPE(type), i, false);
			}
			else
				_currentFormation.push_back(0);
		}
		addPlayer();
		_gameScene->setFormation(_currentFormation);
		return;
	}

	//create preset enemy formation with random type
	for (int i = 0; i < _maskFormation.at(_enemyFormation).size(); i++)
	{
		if (_maskFormation.at(_enemyFormation).at(i))
		{
			int type = random(1, 4);
			_currentFormation.push_back(type);
			setEnemyInPosition(Enemy::ENEMY_TYPE(type), i, false);
		}
		else
			_currentFormation.push_back(0);
	}
	addPlayer();
	_gameScene->setFormation(_currentFormation);
}

void GameManager::restartGame()
{
	auto tryAgain = LabelTTF::create("Try again!", GameSettings::getInstance()->getFontName(), 64);
	tryAgain->setPosition(Point::ZERO);
	this->addChild(tryAgain);
	this->pauseSchedulerAndActions();
	auto restart = CallFuncN::create([](Node* node) {
		auto scene = dynamic_cast<GameManager*>(node)->getGameScene();
		Director::getInstance()->replaceScene(GameScene::createScene(scene->getPlayer(), 0, 3, 100, scene->getFormation()));
	});
	auto delay = DelayTime::create(2);
	this->runAction(Sequence::create(delay, restart, nullptr));
}

Enemy* GameManager::getEnemy(int pos)
{
	return _enemyPosition.at(pos).enemy;
}

void GameManager::addBullet()
{
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();
	auto bullet = Bullet::create(Bullet::PLAYER_BULLET_TYPE);

	bullet->setPosition(_player->getPosition().x, _player->getPosition().y + _player->getContentSize().height / 2 + bullet->getContentSize().height / 2 + 3);

	float angel = MATH_DEG_TO_RAD(random(30,150));

	bullet->getPhysicsBody()->setVelocity(Vec2(cos(angel)*bullet->getSpeed(), sin(angel)*bullet->getSpeed()));

	_bullets.push_back(bullet);
	this->addChild(bullet);
}

void GameManager::addPlayer()
{
	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	_player = Player::creat(_gameScene->getHealth(), _gameScene->getShield());
	_player->setPosition(0, scaleHeight * 90 + _player->getContentSize().height / 2 - this->getContentSize().height / 2);

	auto bullet = Bullet::create(Bullet::PLAYER_BULLET_TYPE);
	bullet->setPosition(_player->getPosition().x, _player->getPosition().y + _player->getContentSize().height / 2 + bullet->getContentSize().height / 2 + 3);
	bullet->getPhysicsBody()->setCollisionBitmask(NO_COLLISION_BITMASK);
	_bullets.push_back(bullet);

	_player->setBulletToShoot(bullet);

	this->addChild(bullet);
	this->addChild(_player);
}

void GameManager::removeEnemy(int pos)
{
	this->removeChild(_enemyPosition.at(pos).enemy);
	_enemyPosition.at(pos).enemy = nullptr;
	_freePosition.push_back(pos);

	// end of game level
	if (_freePosition.size() == 36)
	{
		Director::getInstance()->replaceScene(LevelCompleteScene::createScene(_gameScene->getPlayer(),
																			  _gameScene->getScore() - _gameScene->getStartScore(),
																			  _gameScene->getScore(),
																			  _gameScene->getHealth(),
																			  _gameScene->getShield())
											 );
	}
}

void GameManager::respawnEnemy(Enemy::ENEMY_TYPE type)
{
	if (_freePosition.empty())
		return;

	int index;

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	Size enemySize(84 * scaleWidth, 44 * scaleHeight);
	Size bulletSize(88 * scaleWidth, 88 * scaleHeight);

	//spawn enemy in free position
	while (true)
	{
		index = random(0, int(_freePosition.size() - 1));
		Point position(_enemyPosition.at(_freePosition.at(index)).position.x - enemySize.width / 2 - bulletSize.width / 2, _enemyPosition.at(_freePosition.at(index)).position.y - enemySize.height / 2 - bulletSize.height / 2);
		Rect rect(position.x, position.y, enemySize.width / 2 + bulletSize.width / 2, enemySize.height / 2 + bulletSize.height / 2);
		for (int i = 0; i < _bullets.size(); i++)
			if (!rect.containsPoint(_bullets.at(i)->getPosition()));
				break;
	}

	setEnemyInPosition(type, _freePosition.at(index), true);
}

bool GameManager::onContactBegin(PhysicsContact &contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();


	//////////////////////// Enemy and player bullet collision ///////////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 5)
	{
		SimpleAudioEngine::getInstance()->playEffect("EnemyHit.wav", false, 1.0, 1.0, 1.0);
		Enemy* enemy;
		Bullet* bullet;

		enemy = dynamic_cast<Enemy*>(b->getOwner());
		if (enemy == nullptr)
		{
			enemy = dynamic_cast<Enemy*>(a->getOwner());
			bullet = dynamic_cast<Bullet*>(b->getOwner());
		}
		else
		{
			bullet = dynamic_cast<Bullet*>(a->getOwner());
		}

		enemy->hit(bullet->getDamage());

		if (bullet->getSniperShot())
		{
			float scaleWidth = GameSettings::getInstance()->getWidthScale();
			float scaleHeight = GameSettings::getInstance()->getHeightScale();
			bullet->setDamage(1);
			bullet->setTexture("PlayerBullet.png");
			bullet->setContentSize(Size(bullet->getContentSize().width * scaleWidth, bullet->getContentSize().height * scaleHeight));
			bullet->setSniperShot(false);
		}

		if (bullet->getExtraDamage())
			return false;

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Player and player bullet collision //////////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 65)
	{
		SimpleAudioEngine::getInstance()->playEffect("Hit.wav", false, 1.0, 1.0, 1.0);
		Bullet* bullet;

		bullet = dynamic_cast<Bullet*>(a->getOwner());
		if (bullet == nullptr)
			bullet = dynamic_cast<Bullet*>(b->getOwner());

		float angele = atan(bullet->getPhysicsBody()->getVelocity().y / bullet->getPhysicsBody()->getVelocity().x);
		angele = angele > 0 ? M_PI - angele : angele;
		bullet->getPhysicsBody()->setVelocity(Vec2(bullet->getSpeed() * cos(angele), bullet->getSpeed() * sin(angele)));

		if (_player->getBonus() == Bonus::EXTRA_DAMAGE)
		{
			float scaleWidth = GameSettings::getInstance()->getWidthScale();
			float scaleHeight = GameSettings::getInstance()->getHeightScale();
			bullet->setTexture("PlayerBulletExtra.png");
			bullet->setContentSize(Size(bullet->getContentSize().width * scaleWidth, bullet->getContentSize().height * scaleHeight));
			bullet->setDamage(5);
			bullet->setExtraDamage(true);
			_player->setBonus(Bonus::NO_BONUS);

			auto extra = CallFuncN::create([](Node* node) {
				auto bullet = dynamic_cast<Bullet*>(node);
				float scaleWidth = GameSettings::getInstance()->getWidthScale();
				float scaleHeight = GameSettings::getInstance()->getHeightScale();

				bullet->setDamage(1);
				bullet->setTexture("PlayerBullet.png");
				bullet->setContentSize(Size(bullet->getContentSize().width * scaleWidth, bullet->getContentSize().height * scaleHeight));
				bullet->setExtraDamage(false);
			});

			auto delay = DelayTime::create(5);
			bullet->runAction(Sequence::create(delay, extra, nullptr));
		}

		if (_player->getBonus() == Bonus::SNIPER_SHOT && _player->getBulletToShoot() == nullptr)
		{
			float scaleWidth = GameSettings::getInstance()->getWidthScale();
			float scaleHeight = GameSettings::getInstance()->getHeightScale();

			bullet->setTexture("PlayerBulletSniper.png");
			bullet->setContentSize(Size(bullet->getContentSize().width * scaleWidth, bullet->getContentSize().height * scaleHeight));
			bullet->setSniperShot(true);
			bullet->setDamage(5);
			Point pos(_player->getPosition().x, _player->getPosition().y + _player->getContentSize().height / 2 + bullet->getContentSize().height / 2 + 3);
			bullet->getPhysicsBody()->setCollisionBitmask(NO_COLLISION_BITMASK);
			_player->setBulletToShoot(bullet);
			bullet->getPhysicsBody()->setVelocity(Vec2::ZERO);
			bullet->runAction(Place::create(pos));
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Player and enemy bullet collision //////////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 66)
	{
		SimpleAudioEngine::getInstance()->playEffect("PlayerDamage.wav", false, 1.0, 1.0, 1.0);
		Bullet* bullet;

		bullet = dynamic_cast<Bullet*>(a->getOwner());
		if (bullet == nullptr)
			bullet = dynamic_cast<Bullet*>(b->getOwner());


		_player->hit(bullet->getDamage());

		auto dead = CallFuncN::create([](Node* node) {
			node->removeFromParent();
		});

		auto fadeOut = FadeOut::create(0.1);

		bullet->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
		bullet->runAction(Sequence::create(fadeOut, dead, nullptr));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Enemy bullet and enemy bullet collision /////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 2)
		return false;

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Player bullet and Edge collision ////////////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 257)
	{
		SimpleAudioEngine::getInstance()->playEffect("Hit.wav", false, 1.0, 1.0, 1.0);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Player bullet and player bullet collision ///////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 1)
		return true;

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Player bullet and enemy bullet collision ////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 3)
	{
		Bullet* bullet;

		bullet = dynamic_cast<Bullet*>(a->getOwner());

		if (bullet->getType()==Bullet::PLAYER_BULLET_TYPE)
			bullet = dynamic_cast<Bullet*>(b->getOwner());

		auto dead = CallFuncN::create([](Node* node) {
			node->removeFromParent();
		});

		auto fadeOut = FadeOut::create(0.1);

		bullet->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
		bullet->runAction(Sequence::create(fadeOut, dead, nullptr));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Player and bonus collision //////////////////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 192)
	{
		SimpleAudioEngine::getInstance()->playEffect("Bonus.wav", false, 1.0, 1.0, 1.0);
		Bonus* bonus;

		bonus = dynamic_cast<Bonus*>(a->getOwner());

		if (bonus == nullptr)
			bonus = dynamic_cast<Bonus*>(b->getOwner());

		auto dead = CallFuncN::create([](Node* node) {
			node->removeFromParent();
		});

		if(_player->getBonus() == Bonus::NO_BONUS || bonus->getType() == Bonus::RECOVERY_SHIELD || bonus->getType() == Bonus::RECOVERY_HEALTH)
			_player->setBonus(bonus->getType());
		
		auto fadeOut = FadeOut::create(0.1);

		bonus->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
		bonus->runAction(Sequence::create(fadeOut, dead, nullptr));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Enemy and enemy bullet collision ////////////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 6)
	{
		Bullet* bullet;

		bullet = dynamic_cast<Bullet*>(a->getOwner());

		if (bullet == nullptr)
			bullet = dynamic_cast<Bullet*>(b->getOwner());

		auto dead = CallFuncN::create([](Node* node) {
			node->removeFromParent();
		});

		auto fadeOut = FadeOut::create(0.1);

		bullet->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
		bullet->runAction(Sequence::create(fadeOut, dead, nullptr));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Bottom edge and enemy bullet collision///////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 18)
	{
		Bullet* bullet;

		bullet = dynamic_cast<Bullet*>(a->getOwner());

		if (bullet == nullptr)
			bullet = dynamic_cast<Bullet*>(b->getOwner());

		auto dead = CallFuncN::create([](Node* node) {
			node->removeFromParent();
		});

		auto fadeOut = FadeOut::create(0.1);

		bullet->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
		bullet->runAction(Sequence::create(fadeOut, dead, nullptr));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Bottom edge and bonus collision//////////////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 144)
	{
		Bonus* bonus;

		bonus = dynamic_cast<Bonus*>(a->getOwner());

		if (bonus == nullptr)
			bonus = dynamic_cast<Bonus*>(b->getOwner());

		auto dead = CallFuncN::create([](Node* node) {
			node->removeFromParent();
		});

		auto fadeOut = FadeOut::create(0.1);

		bonus->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
		bonus->runAction(Sequence::create(fadeOut, dead, nullptr));
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Bottom edge and player bullet collision///////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 17)
	{
		Bullet* bullet;

		bullet = dynamic_cast<Bullet*>(a->getOwner());

		if (bullet == nullptr)
			bullet = dynamic_cast<Bullet*>(b->getOwner());

		auto dead = CallFuncN::create([](Node* node) {
			node->removeFromParent();
		});

		_player->hit(101);
		auto fadeOut = FadeOut::create(0.1);

		bullet->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);

		std::vector<Bullet*>::iterator it = _bullets.begin();
		int i;
		for (i = 0; i < _bullets.size(); i++)
			if (_bullets.at(i) == bullet)
				break;
		_bullets.erase(it + i);

		bullet->runAction(Sequence::create(fadeOut, dead, nullptr));

		auto resp = CallFuncN::create([](Node* node) {
			dynamic_cast<GameManager*>(node)->addBullet();
		});

		this->runAction(resp);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Player capture bullet and enemy collision ///////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 36)
	{
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////// Player bullet and enemy field collision /////////////////////////////

	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 9)
	{
		Sprite* field;
		Bullet* bullet;

		bullet = dynamic_cast<Bullet*>(a->getOwner());
		if (bullet == nullptr)
		{
			field = dynamic_cast<Sprite*>(a->getOwner());
			bullet = dynamic_cast<Bullet*>(b->getOwner());
		}
		else
		{
			field = dynamic_cast<Sprite*>(b->getOwner());
		}

		auto enemy = dynamic_cast<Enemy*>(field->getParent());

		field->stopAllActions();
		field->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);

		bullet->getPhysicsBody()->setCollisionBitmask(GameManager::NO_COLLISION_BITMASK);
		bullet->getPhysicsBody()->setVelocity(Vec2::ZERO);

		enemy->setCaptureBullet(bullet);
		enemy->pauseSchedulerAndActions();

		if (_bullets.size() == 1)
			_player->hit(101);

		auto resp = CallFuncN::create([](Node* node) {
			dynamic_cast<GameManager*>(node)->addBullet();
		});

		this->runAction(resp);

		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////

	return false;
}

//void GameManager::onContactSeparate(PhysicsContact &contact)
//{
//	PhysicsBody* a = contact.getShapeA()->getBody();
//	PhysicsBody* b = contact.getShapeB()->getBody();
//
//	//////////////////////// Player and player bullet collision //////////////////////////////////
//
//	if ((a->getCollisionBitmask() | b->getCollisionBitmask()) == 65)
//	{
//		Bullet* bullet;
//		Player* player;
//
//		bullet = dynamic_cast<Bullet*>(a->getOwner());
//
//		if (bullet == nullptr)
//		{
//			bullet = dynamic_cast<Bullet*>(b->getOwner());
//			player = dynamic_cast<Player*>(a->getOwner());
//		}
//		player = dynamic_cast<Player*>(b->getOwner());
//
//		Vec2 velocity(bullet->getPhysicsBody()->getVelocity());
//
//		float angele = atan(bullet->getPhysicsBody()->getVelocity().y / bullet->getPhysicsBody()->getVelocity().x);
//		bullet->getPhysicsBody()->setVelocity(Vec2(bullet->getSpeed() * cos(angele), bullet->getSpeed() * sin(angele)));
//	}
//
//	//////////////////////////////////////////////////////////////////////////////////////////////
//}