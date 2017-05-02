#include "GameSettings.h"

GameSettings* GameSettings::_instance = nullptr;
std::string GameSettings::_fontName = "";

float GameSettings::_widthScale = 1;
float GameSettings::_heightScale = 1;

void GameSettings::init()
{
	if (_instance == nullptr)
		_instance = new GameSettings();
	else
		return;
}

void GameSettings::free()
{
	delete _instance;
	_instance = nullptr;
}