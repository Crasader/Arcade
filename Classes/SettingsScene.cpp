#include "SettingsScene.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "GameSettings.h"

USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;


Scene* SettingsScene::createScene()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float scaleWidth = GameSettings::getInstance()->getWidthScale();
	float scaleHeight = GameSettings::getInstance()->getHeightScale();

	static int musicVolume = UserDefault::getInstance()->getIntegerForKey("musicVolume");
	static int effectVolume = UserDefault::getInstance()->getIntegerForKey("effectVolume");
	static bool mute = UserDefault::getInstance()->getBoolForKey("mute");

	auto scene = Scene::create();

	auto backGround = Sprite::create("Background.png");
	backGround->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	backGround->setContentSize(Size(backGround->getContentSize().width * scaleWidth, backGround->getContentSize().height * scaleHeight));
	scene->addChild(backGround, -1);

	auto soundSettingsLabel = LabelTTF::create("Sounds settings", GameSettings::getInstance()->getFontName(), 64 * scaleHeight);
	soundSettingsLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 150 * scaleHeight));
	scene->addChild(soundSettingsLabel, 0);

	auto musicVolumeLable = LabelTTF::create("Music", GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
	musicVolumeLable->setPosition(Vec2(origin.x + 110 * scaleWidth, origin.y + visibleSize.height / 2 + 30 * scaleHeight));
	scene->addChild(musicVolumeLable, 0);

	auto effectVolumeLabel = LabelTTF::create("Effect", GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
	effectVolumeLabel->setPosition(Vec2(origin.x + 110 * scaleWidth, origin.y + visibleSize.height / 2 - 30 * scaleHeight));
	scene->addChild(effectVolumeLabel, 0);

	auto muteLabel = LabelTTF::create("Mute", GameSettings::getInstance()->getFontName(), 32 * scaleHeight);
	muteLabel->setPosition(Vec2(origin.x + 110 * scaleWidth, origin.y + visibleSize.height / 2 - 90 * scaleHeight));
	scene->addChild(muteLabel, 0);

	auto musicVolumeSlider = Slider::create();
	musicVolumeSlider->loadBarTexture("SliderBack.png");
	musicVolumeSlider->loadSlidBallTextures(
		"SliderNodeNormal.png",
		"SliderNodePress.png",
		"SliderNodeDisable.png"
		);
	musicVolumeSlider->loadProgressBarTexture("SliderProgressBar.png");
	musicVolumeSlider->setPercent(musicVolume);
	musicVolumeSlider->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			SimpleAudioEngine::getInstance()->playEffect("Slide.wav", false, 1.0, 1.0, 1.0);
			break;
		case ui::Widget::TouchEventType::ENDED:
		{
			if (!mute)
				SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(slider->getPercent()) / 100.0);

			musicVolume = slider->getPercent();

			SimpleAudioEngine::getInstance()->playEffect("Slide.wav", false, 1.0, 1.0, 1.0);
			break;
		}
		default:
			break;
		}
	});
	musicVolumeSlider->setContentSize(Size(musicVolumeSlider->getContentSize().width * scaleWidth, musicVolumeSlider->getContentSize().height * scaleHeight));
	musicVolumeSlider->setPosition(Vec2(origin.x + visibleSize.width / 2 + 60 * scaleWidth, origin.y + visibleSize.height / 2 + 28 * scaleHeight));
	scene->addChild(musicVolumeSlider, 1);

	auto effectVolumeSlider = Slider::create();
	effectVolumeSlider->loadBarTexture("SliderBack.png");
	effectVolumeSlider->loadSlidBallTextures(
		"SliderNodeNormal.png",
		"SliderNodePress.png",
		"SliderNodeDisable.png"
		);
	effectVolumeSlider->loadProgressBarTexture("SliderProgressBar.png");
	effectVolumeSlider->setPercent(effectVolume);
	effectVolumeSlider->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			SimpleAudioEngine::getInstance()->playEffect("Slide.wav", false, 1.0, 1.0, 1.0);
			break;
		case ui::Widget::TouchEventType::ENDED:
		{
			if (!mute)
				SimpleAudioEngine::getInstance()->setEffectsVolume(float(slider->getPercent()) / 100.0);

			effectVolume = slider->getPercent();

			SimpleAudioEngine::getInstance()->playEffect("Slide.wav", false, 1.0, 1.0, 1.0);
			break;
		}
		default:
			break;
		}
	});
	effectVolumeSlider->setPosition(Vec2(origin.x + visibleSize.width / 2 + 60 * scaleWidth, origin.y + visibleSize.height / 2 - 29 * scaleHeight));
	effectVolumeSlider->setContentSize(Size(effectVolumeSlider->getContentSize().width * scaleWidth, effectVolumeSlider->getContentSize().height * scaleHeight));
	scene->addChild(effectVolumeSlider, 1);

	auto muteCheckBox = CheckBox::create(
		"CheckBoxNormal.png",
		"CheckBoxPress.png",
		"CheckBoxNodeNormal.png",
		"CheckBoxDisable.png",
		"CheckBoxNodeDisable.png"
		);
	muteCheckBox->setSelectedState(mute);
	muteCheckBox->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
		{
			if (!mute)
			{
				SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
				SimpleAudioEngine::getInstance()->setEffectsVolume(0);
			}
			else
			{
				SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(float(UserDefault::getInstance()->getIntegerForKey("musicVolume")) / 100);
				SimpleAudioEngine::getInstance()->setEffectsVolume(float(UserDefault::getInstance()->getIntegerForKey("effectVolume")) / 100);
			}
			mute = !mute;

			SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
			break;
		}
		case ui::Widget::TouchEventType::ENDED:
			break;
		default:
			break;
		}
	});
	muteCheckBox->setPosition(Vec2(origin.x + visibleSize.width / 2 - 70 * scaleWidth, origin.y + visibleSize.height / 2 - 90 * scaleHeight));
	muteCheckBox->setContentSize(Size(muteCheckBox->getContentSize().width * scaleWidth, muteCheckBox->getContentSize().height * scaleHeight));
	scene->addChild(muteCheckBox, 1);

	auto backItem = MenuItemImage::create(
		"BackNormal.png",
		"BackSelect.png"
		);
	backItem->setCallback([&](Ref* pSender)
	{
		SimpleAudioEngine::getInstance()->playEffect("Click.wav", false, 1.0, 1.0, 1.0);
		UserDefault::getInstance()->setIntegerForKey("musicVolume", musicVolume);
		UserDefault::getInstance()->setIntegerForKey("effectVolume", effectVolume);
		UserDefault::getInstance()->setIntegerForKey("mute", mute);
		Director::getInstance()->popScene();
	});
	backItem->setContentSize(Size(backItem->getContentSize().width * scaleWidth, backItem->getContentSize().height * scaleHeight));
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width / 2 - 3, origin.y + backItem->getContentSize().height / 2 + 3));

	auto menu = Menu::create(backItem, NULL);
	menu->setPosition(Vec2::ZERO);
	scene->addChild(menu, 1);

	return scene;
}