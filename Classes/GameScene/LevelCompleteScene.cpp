#include "LevelCompleteScene.h"
#include "GameScene\GameScene.h"
#include "rapidjson\document.h"
#include "rapidjson\stringbuffer.h"
#include <rapidjson\writer.h>
#include "SimpleAudioEngine.h"
#include "GameSettings.h"

using namespace CocosDenshion;
using namespace rapidjson;

Scene* LevelCompleteScene::createScene(std::string player, int levelScore, int score, int health, int shield)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	auto scene = Scene::create();

	auto visible = CallFuncN::create([](Node* node) {
		node->setVisible(true);
	});

	auto backGround = Sprite::create("Background.png");
	backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	backGround->setScaleX(scaleWidth);
	backGround->setScaleY(scaleHeight);
	scene->addChild(backGround, -1);

	auto levelCompleteLabel = LabelTTF::create("LEVEL COMPLETE", GameSettings::getInstance()->getFontName(), 50 * scaleHeight);
	levelCompleteLabel->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 200 * scaleHeight);
	scene->addChild(levelCompleteLabel);

	auto scoreLevelLabel = LabelTTF::create("Level score:", GameSettings::getInstance()->getFontName(), 40 * scaleHeight);
	scoreLevelLabel->setPosition(origin.x + visibleSize.width / 2 - scoreLevelLabel->getContentSize().width / 2, origin.y + visibleSize.height / 2 + 100 * scaleHeight);
	scene->addChild(scoreLevelLabel);

	auto scoreLevelValueLabel = LabelTTF::create(StringUtils::toString(levelScore), GameSettings::getInstance()->getFontName(), 40 * scaleHeight);
	scoreLevelValueLabel->setPosition(origin.x + visibleSize.width / 2 - scoreLevelValueLabel->getContentSize().width / 2 + 80 * scaleWidth, origin.y + visibleSize.height / 2 + 100 *scaleHeight);
	scoreLevelValueLabel->setScale(6);
	scoreLevelValueLabel->setVisible(false);
	scene->addChild(scoreLevelValueLabel);
	scoreLevelValueLabel->runAction(Sequence::create(DelayTime::create(1), visible, ScaleTo::create(1, 1), nullptr));

	auto scoreLabel = LabelTTF::create("Total score:", GameSettings::getInstance()->getFontName(), 40 * scaleHeight);
	scoreLabel->setPosition(origin.x + visibleSize.width / 2 - scoreLabel->getContentSize().width / 2, origin.y + visibleSize.height / 2 );
	scene->addChild(scoreLabel);

	auto scoreValueLabel = LabelTTF::create(StringUtils::toString(score), GameSettings::getInstance()->getFontName(), 40 * scaleHeight);
	scoreValueLabel->setPosition(origin.x + visibleSize.width / 2 - scoreValueLabel->getContentSize().width / 2 + 80 * scaleWidth, origin.y + visibleSize.height / 2);
	scoreValueLabel->setScale(6);
	scoreValueLabel->setVisible(false);
	scene->addChild(scoreValueLabel);
	scoreValueLabel->runAction(Sequence::create(DelayTime::create(2.5), visible, ScaleTo::create(1, 1), nullptr));

	auto nextItem = MenuItemImage::create(
		"NextLevelNormal.png",
		"NextLevelSelect.png"
		);
	nextItem->setCallback([=](Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		Director::getInstance()->replaceScene(GameScene::createScene(player, score, health, shield));
	});
	nextItem->setPosition(origin.x + visibleSize.width - nextItem->getContentSize().width / 2 * scaleWidth - 3, origin.y + nextItem->getContentSize().height / 2 * scaleHeight + 3);
	nextItem->setScaleX(scaleWidth);
	nextItem->setScaleY(scaleHeight);
	nextItem->setVisible(false);
	nextItem->runAction(Sequence::create(DelayTime::create(4), visible, nullptr));

	auto mainMenuItem = MenuItemImage::create(
		"MainMenuNormal.png",
		"MainMenuSelect.png"
		);
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
			memset(buffer, 0, sizeof(buffer));
		}

		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		leaderboardJson.Accept(writer);
		const char* output = buffer.GetString();
		FileUtils::getInstance()->writeStringToFile(output, path);

		SimpleAudioEngine::getInstance()->playBackgroundMusic("MenuMusic.mp3", true);
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);

		Director::getInstance()->popScene();
	});
	mainMenuItem->setPosition(Vec2(origin.x + mainMenuItem->getContentSize().width / 2 * scaleWidth + 3, origin.y + mainMenuItem->getContentSize().height / 2 * scaleHeight + 3));
	mainMenuItem->setScaleX(scaleWidth);
	mainMenuItem->setScaleY(scaleHeight);
	mainMenuItem->setVisible(false);
	mainMenuItem->runAction(Sequence::create(DelayTime::create(4), visible, nullptr));

	auto menu = Menu::create(nextItem, mainMenuItem, NULL);
	menu->setPosition(Vec2::ZERO);
	scene->addChild(menu);

	return scene;
}