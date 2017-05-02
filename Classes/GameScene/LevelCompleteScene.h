#ifndef _LEVEL_COMPLETE_SCENE_H_
#define _LEVEL_COMPLETE_SCENE_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

class LevelCompleteScene : public Ref
{
public:
	static Scene* createScene(std::string player, int levelScore, int score, int health, int shield);
};

#endif // !_LEVEL_COMPLETE_SCENE_H_
