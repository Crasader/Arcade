#include "MainScene.h"
#include "CloseScene.h"
#include "SettingsScene.h"
#include "LeaderboardScene.h"
#include "EnterPlayerScene.h"
#include "SimpleAudioEngine.h"
#include "GameSettings.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    //super init first
    if ( !Layer::init() )
    {
        return false;
    }

	GameSettings::getInstance()->setFontName("Marker Felt.ttf");
    auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	UserDefault::getInstance()->setIntegerForKey("musicVolume", 50);
	UserDefault::getInstance()->setIntegerForKey("effectVolume", 50);
	UserDefault::getInstance()->setBoolForKey("mute", false);

    /////////////////////////////
	//Background and game menu

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	auto backGround = Sprite::create("Background.png");
	backGround->setContentSize(Size(backGround->getContentSize().width * scaleWidth, backGround->getContentSize().height * scaleHeight));
	backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(backGround, -1);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("MenuMusic.mp3", true);

	auto startLabel = Label::create("Start", GameSettings::getInstance()->getFontName(), 64 * scaleHeight);
	auto startItemLabel = MenuItemLabel::create(startLabel);
	startItemLabel->setCallback([&](Ref* pSender) 
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->pushScene(EnterPlayerScene::createScene());
	});
	startItemLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + startLabel->getContentSize().height / 2 + 10));

	auto leaderboardLabel = Label::create("Leaderboard", GameSettings::getInstance()->getFontName(), 64 * scaleHeight);
	auto leaderboardItemLabel = MenuItemLabel::create(leaderboardLabel);
	leaderboardItemLabel->setCallback([&](Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->pushScene(LeaderboardScene::createScene());
	});
	leaderboardItemLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - leaderboardLabel->getContentSize().height / 2 - 10));

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelect.png"
		);
	closeItem->setCallback([&](Ref* pSender) 
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->pushScene(CloseScene::createScene());
	});
	closeItem->setContentSize(Size(closeItem->getContentSize().width * scaleWidth, closeItem->getContentSize().height * scaleHeight));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2 - 3, origin.y + closeItem->getContentSize().height / 2 + 3));

	auto settingsItem = MenuItemImage::create(
		"SettingsNormal.png",
		"SettingsSelect.png"
		);
	settingsItem->setCallback([&](Ref* pSender) 
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->pushScene(SettingsScene::createScene());
	});
	settingsItem->setContentSize(Size(settingsItem->getContentSize().width * scaleWidth, settingsItem->getContentSize().height * scaleHeight));
	settingsItem->setPosition(Vec2(origin.x + settingsItem->getContentSize().width / 2 + 3, origin.y + settingsItem->getContentSize().height / 2 + 3));

	auto menu = Menu::create(closeItem, settingsItem, startItemLabel, leaderboardItemLabel, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    return true;
}