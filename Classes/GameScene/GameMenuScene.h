#ifndef _GAME_MENU_SCENE_H_
#define _GAME_MENU_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class GameMenuScene : public Ref
{
public:
	static Scene* createScene(std::string player, int score);
};
#endif // !_GAME_MENU_SCENE_H_
