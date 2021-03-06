#include "AppDelegate.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "GameSettings.h"

USING_NS_CC;
using namespace CocosDenshion;

static cocos2d::Size designResolutionSize = cocos2d::Size(540, 960);
//static cocos2d::Size designResolutionSize = cocos2d::Size(768, 1280);


AppDelegate::AppDelegate()
{
	GameSettings::init();
}

AppDelegate::~AppDelegate() 
{
	GameSettings::free();
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("GameTest", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("GameTest");
#endif
        director->setOpenGLView(glview);
    }

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

	FileUtils::getInstance()->addSearchPath("MainScene");
	FileUtils::getInstance()->addSearchPath("SettingsScene");
	FileUtils::getInstance()->addSearchPath("GameScene");
	FileUtils::getInstance()->addSearchPath("fonts");
	FileUtils::getInstance()->addSearchPath("EnterPlayerScene");
	FileUtils::getInstance()->addSearchPath("GameScene/Sounds");

	

	// Set the design resolution
	//glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	auto frameSize = glview->getFrameSize();
	glview->setDesignResolutionSize(frameSize.width, frameSize.height, ResolutionPolicy::NO_BORDER);

	GameSettings::getInstance()->setWidthScale(frameSize.width / designResolutionSize.width);
	GameSettings::getInstance()->setHeightScale(frameSize.height / designResolutionSize.height);

    // create a scene. it's an autorelease object
    auto scene = MainScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
