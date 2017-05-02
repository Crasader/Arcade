#include "EnterPlayerScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameScene/GameScene.h"
#include "GameSettings.h";

using namespace ui;
using namespace CocosDenshion;

std::string player = "";

Scene* EnterPlayerScene::createScene()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	player = "";

	auto scene = Scene::create();

	auto backGround = Sprite::create("Background.png");
	backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	backGround->setContentSize(Size(backGround->getContentSize().width * scaleWidth, backGround->getContentSize().height * scaleHeight));
	scene->addChild(backGround, -1);

	auto soundSettingsLabel = LabelTTF::create("Enter player name:", GameSettings::getInstance()->getFontName(), 50 * scaleHeight);
	soundSettingsLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 80 * scaleHeight));
	scene->addChild(soundSettingsLabel, 0);

	auto playerTextField = TextField::create("Player", GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
	playerTextField->setMaxLength(12);
	playerTextField->setMaxLengthEnabled(true);
	playerTextField->addEventListener([&](Ref* pSender, TextField::EventType type)
	{
		auto textField = dynamic_cast<TextField*>(pSender);

		switch (type)
		{
		case cocos2d::ui::TextField::EventType::INSERT_TEXT:
			player = textField->getString();
			break;
		default:
			break;
		}

	});
	playerTextField->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	scene->addChild(playerTextField, 1);

	auto backItem = MenuItemImage::create(
		"BackNormal.png",
		"BackSelect.png"
		);
	backItem->setCallback([&](Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->popScene();
	});
	backItem->setContentSize(Size(backItem->getContentSize().width * scaleWidth, backItem->getContentSize().height * scaleHeight));
	backItem->setPosition(Vec2(origin.x + backItem->getContentSize().width / 2 + 3, origin.y + backItem->getContentSize().height / 2 + 3));

	auto nextItem = MenuItemImage::create(
		"NextLevelNormal.png",
		"NextLevelSelect.png"
		);
	nextItem->setCallback([&](Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		if (player != "")
		{
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			Director::getInstance()->replaceScene(GameScene::createScene(player, 0, 3, 100));
		}
	});
	nextItem->setContentSize(Size(nextItem->getContentSize().width * scaleWidth, nextItem->getContentSize().height * scaleHeight));
	nextItem->setPosition(Vec2(origin.x + visibleSize.width - nextItem->getContentSize().width / 2 - 3, origin.y + nextItem->getContentSize().height / 2 + 3));

	auto menu = Menu::create(backItem, nextItem, NULL);
	menu->setPosition(Vec2::ZERO);
	scene->addChild(menu, 1);

	return scene;
}