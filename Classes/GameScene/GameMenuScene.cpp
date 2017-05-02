#include "GameMenuScene.h"
#include "SettingsScene.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "ui\CocosGUI.h"
#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include "GameSettings.h"

using namespace CocosDenshion;
using namespace ui;
using namespace rapidjson;

Scene* GameMenuScene::createScene(std::string player, int score)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	auto scene = Scene::create();

	auto backGround = Sprite::create("Background.png");
	backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	backGround->setScaleX(scaleWidth);
	backGround->setScaleY(scaleHeight);
	scene->addChild(backGround, -1);

	auto settingsLabel = Label::create("Settings", GameSettings::getInstance()->getFontName(), 64);
	auto settingsItem = MenuItemLabel::create(settingsLabel);
	settingsItem->setCallback([&](Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->pushScene(SettingsScene::createScene());
	});
	settingsItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 35 * scaleHeight));
	settingsItem->setScaleX(scaleWidth);
	settingsItem->setScaleY(scaleHeight);

	auto mainMenuLabel = Label::create("Main Menu", GameSettings::getInstance()->getFontName(), 64);
	auto mainMenuItem = MenuItemLabel::create(mainMenuLabel);
	mainMenuItem->setCallback([=](Ref* pSender)
	{
		Document leaderboardJson;
		std::string path = FileUtils::getInstance()->getWritablePath() + "leaderboard.json";
		if (FileUtils::getInstance()->isFileExist(path))
		{
			std::string leaderboardString = FileUtils::getInstance()->getStringFromFile(path);
			leaderboardJson.Parse(leaderboardString.c_str());

			std::vector<std::pair<std::string, int>> leaderboard;

			for (auto it = leaderboardJson.MemberBegin(); it != leaderboardJson.MemberEnd(); it++)
				leaderboard.push_back(std::pair<std::string, int>(it->name.GetString(), it->value.GetInt()));

			std::vector<std::pair<std::string, int>>::iterator it;

			for (it = leaderboard.begin(); it != leaderboard.end(); it++)
				if (it->first == player)
					break;

			if (it != leaderboard.end())
				it->second = score;
			else
				leaderboard.push_back(std::pair<std::string, int>(player, score));

			std::sort(leaderboard.begin(), leaderboard.end(), [](std::pair<std::string, int> a, std::pair<std::string, int> b) {
				return a.second > b.second;
			});

			leaderboardJson.RemoveAllMembers();

			for (it = leaderboard.begin(); it != leaderboard.end(); it++)
			{
				char buffer[13];
				it->first.copy(buffer, it->first.length());
				rapidjson::Value str;
				str.SetString(buffer, it->first.length(), leaderboardJson.GetAllocator());
				leaderboardJson.AddMember(str, it->second, leaderboardJson.GetAllocator());
				memset(buffer, 0, sizeof(buffer));
			}
		}
		else
		{
			char buffer[13];
			player.copy(buffer, player.length());
			rapidjson::Value str;
			leaderboardJson.SetObject();
			str.SetString(buffer, player.length(), leaderboardJson.GetAllocator());
			leaderboardJson.AddMember(str, score, leaderboardJson.GetAllocator());
		}

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		leaderboardJson.Accept(writer);
		const char* output = buffer.GetString();
		FileUtils::getInstance()->writeStringToFile(output, path);

		SimpleAudioEngine::getInstance()->playBackgroundMusic("MenuMusic.mp3", true);
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->popToRootScene();
	});
	mainMenuItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 35 * scaleHeight));
	mainMenuItem->setScaleX(scaleWidth);
	mainMenuItem->setScaleY(scaleHeight);

	auto backItem = MenuItemImage::create(
		"BackNormal.png",
		"BackSelect.png"
		);
	backItem->setCallback([&](Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->popScene();
	});
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width / 2* scaleWidth - 3, origin.y + backItem->getContentSize().height / 2 * scaleHeight + 3));
	backItem->setScaleX(scaleWidth);
	backItem->setScaleY(scaleHeight);

	auto menu = Menu::create(mainMenuItem, settingsItem, backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	scene->addChild(menu, 1);

	return scene;
}