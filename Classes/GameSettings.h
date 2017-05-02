#ifndef _GAME_SETTINGS_H_
#define _GAME_SETTINGS_H_

#include "cocos2d.h"

class GameSettings
{
public:
	static GameSettings* getInstance() { return _instance; }

	static void init();
	static void free();

	const std::string getFontName() { return _fontName; }
	void setFontName(std::string name) { _fontName = name; }

	void setWidthScale(float scale) { _widthScale = scale; }
	float getWidthScale() { return _widthScale; }

	void setHeightScale(float scale) { _heightScale = scale; }
	float getHeightScale() { return _heightScale; }

private:
	GameSettings() {};
	~GameSettings() {};

	static float _widthScale;
	static float _heightScale;

	static GameSettings* _instance;
	static std::string _fontName;
};

#endif // !_GAME_FONT_H_
