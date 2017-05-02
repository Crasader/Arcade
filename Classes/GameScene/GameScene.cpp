#include "GameScene.h"
#include "GameMenuScene.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GameManager.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "GameSettings.h"

using namespace CocosDenshion;
using namespace ui;

GameScene* GameScene::createScene(std::string player, int score, int health, int shield)
{
	GameScene *ret = new (std::nothrow) GameScene();
	if (ret && ret->init(player, score, health, shield))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

GameScene* GameScene::createScene(std::string player, int score, int health, int shield, std::vector<int> formation)
{
	GameScene *ret = new (std::nothrow) GameScene();
	if (ret && ret->init(player, score, health, shield, formation))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

bool GameScene::init(std::string player, int score, int health, int shield)
{
	if (initWithPhysics())
	{
		_player = player;
		_score = score;
		_startScore = score;
		_health = health;
		_shield = shield;
		_formation = { 0, 0, 0, 0, 0, 0,
			           0, 0, 0, 0, 0, 0,
			           0, 0, 0, 0, 0, 0,
			           0, 0, 0, 0, 0, 0,
			           0, 0, 0, 0, 0, 0,
			           0, 0, 0, 0, 0, 0
		};

		if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
			SimpleAudioEngine::getInstance()->playBackgroundMusic("GameMusic.mp3", true);

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		float scaleWidth = GameSettings::getInstance()->getWidthScale();
		float scaleHeight = GameSettings::getInstance()->getHeightScale();

		this->getPhysicsWorld()->setGravity(Vec2(0, 0));
		//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

		auto backGround = Sprite::create("Background.png");
		backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
		backGround->setContentSize(Size(backGround->getContentSize().width * scaleWidth, backGround->getContentSize().height * scaleHeight));
		this->addChild(backGround, -1);

		auto uiNode = Node::create();
		uiNode->setName("ui");

		auto playerLabel = LabelTTF::create(_player, GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
		playerLabel->setPosition(Point(origin.x + playerLabel->getContentSize().width / 2 + 3, origin.y + visibleSize.height - playerLabel->getContentSize().height / 2 - 10));
		playerLabel->setName("player");
		uiNode->addChild(playerLabel);

		auto scoreLabel = LabelTTF::create(StringUtils::toString(_score), GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
		scoreLabel->setPosition(Point(origin.x + playerLabel->getContentSize().width + scoreLabel->getContentSize().width / 2 + 30 * scaleWidth, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		scoreLabel->setName("score");
		uiNode->addChild(scoreLabel);

		auto menuItem = MenuItemImage::create(
			"MenuNormal.png",
			"MenuSelect.png"
			);
		menuItem->setCallback(CC_CALLBACK_0(GameScene::gameMenuCallBack,this));
		menuItem->setContentSize(Size(menuItem->getContentSize().width * scaleWidth, menuItem->getContentSize().height * scaleHeight));
		menuItem->setPosition(Point(origin.x + visibleSize.width - menuItem->getContentSize().width / 2 - 3, origin.y + visibleSize.height - menuItem->getContentSize().height / 2 - 3));

		auto menu = Menu::create(menuItem, NULL);
		menu->setPosition(Point(Size::ZERO));
		uiNode->addChild(menu);

		auto shieldCountLabel = LabelTTF::create(StringUtils::toString(_shield), GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
		shieldCountLabel->setDimensions(Size(64 * scaleWidth, 36 * scaleHeight));
		shieldCountLabel->setPosition(Point(origin.x + visibleSize.width - shieldCountLabel->getContentSize().width / 2 - menuItem->getContentSize().width - 30 * scaleWidth, origin.y + visibleSize.height - shieldCountLabel->getContentSize().height / 2 - 10));
		shieldCountLabel->setName("shield");
		uiNode->addChild(shieldCountLabel);

		auto shieldIcon = Sprite::create("ShieldIcon.png");
		shieldIcon->setContentSize(Size(shieldIcon->getContentSize().width * scaleWidth, shieldIcon->getContentSize().height * scaleHeight));
		shieldIcon->setPosition(Point(shieldCountLabel->getPosition().x - shieldCountLabel->getContentSize().width, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		uiNode->addChild(shieldIcon);

		auto healthCountLabel = LabelTTF::create(StringUtils::toString(_health), GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
		healthCountLabel->setPosition(Point(shieldIcon->getPosition().x - shieldIcon->getContentSize().width * scaleWidth, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		healthCountLabel->setName("health");
		uiNode->addChild(healthCountLabel);

		auto healthIcon = Sprite::create("HealthIcon.png");
		healthIcon->setContentSize(Size(healthIcon->getContentSize().width * scaleWidth, healthIcon->getContentSize().height * scaleHeight));
		healthIcon->setPosition(Point(healthCountLabel->getPosition().x - healthCountLabel->getContentSize().width - healthCountLabel->getContentSize().width - 5 * scaleWidth, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		uiNode->addChild(healthIcon);

		this->addChild(uiNode, 0);

		auto manager = GameManager::create(this);
		manager->setName("manager");

		manager->setEnemyFormation(GameManager::ENEMY_FORMATION(random(0,4)));
		manager->runGame();
		this->addChild(manager);

		return true;
	}
	return false;
}

bool GameScene::init(std::string player, int score, int health, int shield, std::vector<int> formation)
{
	if (initWithPhysics())
	{
		_player = player;
		_score = score;
		_startScore = score;
		_health = health;
		_shield = shield;
		_formation = formation;

		if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
			SimpleAudioEngine::getInstance()->playBackgroundMusic("GameMusic.mp3", true);

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		float scaleWidth = GameSettings::getInstance()->getWidthScale();
		float scaleHeight = GameSettings::getInstance()->getHeightScale();

		this->getPhysicsWorld()->setGravity(Vec2(0, 0));
		//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

		auto backGround = Sprite::create("Background.png");
		backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
		backGround->setContentSize(Size(backGround->getContentSize().width * scaleWidth, backGround->getContentSize().height * scaleHeight));
		this->addChild(backGround, -1);

		auto uiNode = Node::create();
		uiNode->setName("ui");

		auto playerLabel = LabelTTF::create(_player, GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
		playerLabel->setPosition(Point(origin.x + playerLabel->getContentSize().width / 2 + 3, origin.y + visibleSize.height - playerLabel->getContentSize().height / 2 - 10));
		playerLabel->setName("player");
		uiNode->addChild(playerLabel);

		auto scoreLabel = LabelTTF::create(StringUtils::toString(_score), GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
		scoreLabel->setPosition(Point(origin.x + playerLabel->getContentSize().width + scoreLabel->getContentSize().width / 2 + 30 * scaleWidth, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		scoreLabel->setName("score");
		uiNode->addChild(scoreLabel);

		auto menuItem = MenuItemImage::create(
			"MenuNormal.png",
			"MenuSelect.png"
			);
		menuItem->setCallback(CC_CALLBACK_0(GameScene::gameMenuCallBack, this));
		menuItem->setContentSize(Size(menuItem->getContentSize().width * scaleWidth, menuItem->getContentSize().height * scaleHeight));
		menuItem->setPosition(Point(origin.x + visibleSize.width - menuItem->getContentSize().width / 2 - 3, origin.y + visibleSize.height - menuItem->getContentSize().height / 2 - 3));

		auto menu = Menu::create(menuItem, NULL);
		menu->setPosition(Point(Size::ZERO));
		uiNode->addChild(menu);

		auto shieldCountLabel = LabelTTF::create(StringUtils::toString(_shield), GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
		shieldCountLabel->setDimensions(Size(64 * scaleWidth, 36 * scaleHeight));
		shieldCountLabel->setPosition(Point(origin.x + visibleSize.width - shieldCountLabel->getContentSize().width / 2 - menuItem->getContentSize().width - 30 * scaleWidth, origin.y + visibleSize.height - shieldCountLabel->getContentSize().height / 2 - 10));
		shieldCountLabel->setName("shield");
		uiNode->addChild(shieldCountLabel);

		auto shieldIcon = Sprite::create("ShieldIcon.png");
		shieldIcon->setContentSize(Size(shieldIcon->getContentSize().width * scaleWidth, shieldIcon->getContentSize().height * scaleHeight));
		shieldIcon->setPosition(Point(shieldCountLabel->getPosition().x - shieldCountLabel->getContentSize().width, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		uiNode->addChild(shieldIcon);

		auto healthCountLabel = LabelTTF::create(StringUtils::toString(_health), GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
		healthCountLabel->setPosition(Point(shieldIcon->getPosition().x - shieldIcon->getContentSize().width * scaleWidth, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		healthCountLabel->setName("health");
		uiNode->addChild(healthCountLabel);

		auto healthIcon = Sprite::create("HealthIcon.png");
		healthIcon->setContentSize(Size(healthIcon->getContentSize().width * scaleWidth, healthIcon->getContentSize().height * scaleHeight));
		healthIcon->setPosition(Point(healthCountLabel->getPosition().x - healthCountLabel->getContentSize().width - healthCountLabel->getContentSize().width - 5 * scaleWidth, origin.y + visibleSize.height - scoreLabel->getContentSize().height / 2 - 10));
		uiNode->addChild(healthIcon);

		this->addChild(uiNode, 0);

		auto manager = GameManager::create(this);
		manager->setName("manager");

		manager->setCustomFormationWhithEnemyType(_formation);
		manager->runGame();
		this->addChild(manager);

		return true;
	}
	return false;
}

void GameScene::setScore(int score)
{
	_score = score;
	auto label = dynamic_cast<LabelTTF*>(this->getChildByName("ui")->getChildByName("score"));
	label->setString(StringUtils::toString(_score));
}

void GameScene::changeScore(int score)
{
	_score += score;
	auto label = dynamic_cast<LabelTTF*>(this->getChildByName("ui")->getChildByName("score"));
	label->setString(StringUtils::toString(_score));
	label->runAction(Sequence::create(ScaleTo::create(0.1, 1.5), ScaleTo::create(0.1, 1), nullptr));
}

void GameScene::setHealth(int health)
{
	_health = health;
	auto label = dynamic_cast<LabelTTF*>(this->getChildByName("ui")->getChildByName("health"));
	label->setString(StringUtils::toString(_health));
}


void GameScene::changeHealth(int health)
{
	SimpleAudioEngine::getInstance()->playEffect("HealthHit.wav", false, 1.0, 1.0, 1.0);
	_health += health;
	auto label = dynamic_cast<LabelTTF*>(this->getChildByName("ui")->getChildByName("health"));
	label->setString(StringUtils::toString(_health));
	label->runAction(Sequence::create(ScaleTo::create(0.1, 1.5), ScaleTo::create(0.1, 1), nullptr));
}

void GameScene::setShield(int shield)
{
	_shield = shield;
	auto label = dynamic_cast<LabelTTF*>(this->getChildByName("ui")->getChildByName("shield"));
	label->setString(StringUtils::toString(_shield));
}


void GameScene::changeShield(int shield)
{
	if(_shield == 0 && shield >0)
		SimpleAudioEngine::getInstance()->playEffect("ShieldRecovery.mp3", false, 1.0, 1.0, 1.0);

	_shield += shield;

	if (_shield == 0)
		SimpleAudioEngine::getInstance()->playEffect("ShieldDestroy.mp3", false, 1.0, 1.0, 1.0);

	auto label = dynamic_cast<LabelTTF*>(this->getChildByName("ui")->getChildByName("shield"));
	label->setString(StringUtils::toString(_shield));
	label->runAction(Sequence::create(ScaleTo::create(0.1, 1.5), ScaleTo::create(0.1, 1), nullptr));
}

void GameScene::gameMenuCallBack()
{
	SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
	Director::getInstance()->pushScene(GameMenuScene::createScene(_player,_score));
}