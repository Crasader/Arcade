#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public Scene
{
public:
	// create game with random enemy formation
	static GameScene* createScene(std::string player, int score, int health, int shield);

	// create game with custom enemy formation
	static GameScene* createScene(std::string player, int score, int health, int shield, std::vector<int> formation);

	bool init(std::string player, int score, int health, int shield);
	bool init(std::string player, int score, int health, int shield, std::vector<int> formation);

	void setScore(int score);
	int getScore() { return _score; }
	void changeScore(int score);

	int getStartScore() { return _startScore; }

	void setHealth(int health);
	int getHealth() { return _health; }
	void changeHealth(int health);

	void setShield(int health);
	int getShield() { return _shield; }
	void changeShield(int health);

	void setFormation(std::vector<int> form) { _formation = form; }
	std::vector<int> getFormation() { return _formation; }

	std::string getPlayer() { return _player; }

private:
	// player's name
	std::string _player;

	// player's score
	int _score;

	// player's score in the beginning of the level
	int _startScore;

	// player's health ponts
	int _health;

	// player's shield points
	int _shield;

	// current enemy formation with type
	std::vector<int> _formation;

	void gameMenuCallBack();
};

#endif // !_GAME_SCENE_H_

