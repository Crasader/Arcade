#include "LeaderboardScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "rapidjson\document.h"
#include "GameSettings.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;
using namespace rapidjson;

Scene* LeaderboardScene::createScene()
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

	auto leaderboardLabel = LabelTTF::create("Leaderboard", GameSettings::getInstance()->getFontName(), 64 * scaleHeight);
	leaderboardLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 100 * scaleHeight));
	scene->addChild(leaderboardLabel, 0);

	auto playerLabel = LabelTTF::create("Player", GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
	playerLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 - 150 * scaleWidth, origin.y + visibleSize.height / 2 + 300 * scaleHeight));
	scene->addChild(playerLabel, 0);

	auto scoreLabel = LabelTTF::create("Score", GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
	scoreLabel->setPosition(Vec2(origin.x + visibleSize.width / 2 + 150 * scaleWidth, origin.y + visibleSize.height / 2 + 300 * scaleHeight));
	scene->addChild(scoreLabel, 0);
	
	std::string path =  FileUtils::getInstance()->getWritablePath() + "leaderboard.json";

	if (FileUtils::getInstance()->isFileExist(path))
	{
		Document leaderboardJson;
		std::string leaderboardString = FileUtils::getInstance()->getStringFromFile(path);
		leaderboardJson.Parse(leaderboardString.c_str());

		auto leaderScrollView = ScrollView::create();
		leaderScrollView->setDirection(ScrollView::Direction::VERTICAL);
		leaderScrollView->setBounceEnabled(true);
		leaderScrollView->setAnchorPoint(Vec2(0.5, 0.5));

		Size leaderboardMaxSize(visibleSize.width - 100 * scaleWidth, visibleSize.height - 400 * scaleHeight);
		Size leaderboardSize(visibleSize.width - 100 * scaleWidth, leaderboardJson.MemberCount() * 40 * scaleHeight);

		if (leaderboardSize.height > leaderboardMaxSize.height)
			leaderScrollView->setContentSize(leaderboardMaxSize);
		else
			leaderScrollView->setContentSize(leaderboardSize);

		leaderScrollView->setInnerContainerSize(leaderboardSize);

		leaderScrollView->setPosition(Vec2(origin.x + visibleSize.width / 2, scoreLabel->getPosition().y - scaleHeight * scoreLabel->getContentSize().height - leaderScrollView->getContentSize().height / 2));

		rapidjson::Value::MemberIterator it;
		int i;
		for (it = leaderboardJson.MemberBegin(), i = 0; it != leaderboardJson.MemberEnd(); it++, i++)
		{
			auto player = LabelTTF::create(it->name.GetString(), GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
			player->setPosition(Vec2(origin.x + player->getContentSize().width / 2, leaderScrollView->getInnerContainerSize().height - player->getContentSize().height / 2 - i * 40 * scaleHeight));
			leaderScrollView->addChild(player);

			auto score = LabelTTF::create(StringUtils::toString(it->value.GetInt()), GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
			score->setPosition(Vec2(origin.x + leaderScrollView->getContentSize().width - score->getContentSize().width / 2, leaderScrollView->getInnerContainerSize().height - player->getContentSize().height / 2 - i * 40 * scaleHeight));
			leaderScrollView->addChild(score);
		}
		scene->addChild(leaderScrollView, 1);
	}

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
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width / 2 - 3, origin.y + backItem->getContentSize().height / 2 + 3));

	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	scene->addChild(menu, 1);

	return scene;
}
