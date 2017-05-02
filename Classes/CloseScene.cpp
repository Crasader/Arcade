#include "CloseScene.h"
#include "SimpleAudioEngine.h"
#include "GameSettings.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* CloseScene::createScene()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	auto scene = Scene::create();

	auto backGround = Sprite::create("Background.png");
	backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	backGround->setContentSize(Size(backGround->getContentSize().width * scaleWidth, backGround->getContentSize().height * scaleHeight));
	scene->addChild(backGround, -1);

	auto itemOK = MenuItemImage::create(
		"OKNormal.png",
		"OKSelect.png"
		);
	itemOK->setCallback([&](Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->end();
	});
	itemOK->setContentSize(Size(itemOK->getContentSize().width * scaleWidth, itemOK->getContentSize().height * scaleHeight));
	itemOK->setPosition(Vec2(origin.x + visibleSize.width / 2 - itemOK->getContentSize().width / 2 - 30 * scaleWidth, origin.y + visibleSize.height / 2 - 30 * scaleHeight));

	auto itemCancel = MenuItemImage::create(
		"CancelNormal.png",
		"CancelSelect.png"
		);
	itemCancel->setCallback([&](Ref* pSener) 
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->popScene(); 
	});
	itemCancel->setContentSize(Size(itemCancel->getContentSize().width * scaleWidth, itemCancel->getContentSize().height * scaleHeight));
	itemCancel->setPosition(Vec2(origin.x + visibleSize.width / 2 + itemCancel->getContentSize().width / 2 + 30 * scaleWidth, origin.y + visibleSize.height / 2 - 30 * scaleHeight));

	auto quitLabel = Label::create("Are you sure?", GameSettings::getInstance()->getFontName(), 64 * scaleHeight);
	quitLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100 * scaleHeight));
	scene->addChild(quitLabel, 0);

	auto menu = Menu::create(itemOK, itemCancel, NULL);
	menu->setPosition(Vec2::ZERO);
	scene->addChild(menu, 1);

	return scene;
}