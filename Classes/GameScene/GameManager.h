#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "cocos2d.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"
#include "GameScene.h"

USING_NS_CC;

class GameManager : public Node
{
public:
	enum ENEMY_FORMATION
	{
		PHALANX_FORMATION,
		WEDGE_FORMATION,
		SQUARE_FORMATION,
		REVERSE_WEDGE_FORMATION,
		RANDOM_FORMATION
	};

	enum COLLISION_BITMASK
	{
		PLAYER_BULLET_BITMASK = 1,
		ENEMY_BULLET_BITMASK = 2,
		ENEMY_BITMASK = 4,
		ENEMY_FIELD_BITMASK = 8,
		BOTTOM_EDGE_BITMASK = 16,
		ENEMY_BULLET_CAPTURE_BITMASK = 32,
		PLAYER_BITMASK = 64,
		BONUS_BITMASK = 128,
		EDGE_BITMASK = 256,
		NO_COLLISION_BITMASK = 512
	};

	static GameManager* create(GameScene* scene);
	bool init(GameScene* scene);

	GameScene* getGameScene() { return _gameScene; }

	void setEnemyFormation(ENEMY_FORMATION form);
	void setCustomEnemyFormation(std::vector<int> form);
	void setCustomFormationWhithEnemyType(std::vector<int> form);
	void setEnemyInPosition(Enemy::ENEMY_TYPE type, int pos, bool res);

	void respawnEnemy(Enemy::ENEMY_TYPE type);
	Enemy* getEnemy(int pos);

	Player* getPlayer() { return _player; }

	void runGame();
	void restartGame();

	void newRandomFormation();
	void removeEnemy(int pos);

	void addBullet();
private:
	GameScene* _gameScene;

	struct enemyPosition
	{
		Enemy* enemy;
		Point position;
	};

	bool onContactBegin(PhysicsContact &contact);

	void addPlayer();

	Player* _player;

	// array of enemies position
	std::vector<enemyPosition> _enemyPosition;
	// arra of free enemies position
	std::vector<int> _freePosition;

	// array of player's bullets
	std::vector<Bullet*> _bullets;

	// preset mask of enemy formation
	std::vector<std::vector<int>> _maskFormation;

	// enemy formation
	ENEMY_FORMATION _enemyFormation;

	// custom enemy formation
	std::vector<int> _customEnemyFormation;
	bool _isCustomEnemyFormation;

	// custom enemy formation with enemy type
	std::vector<int> _customFormationWithEnemyType;
	bool _isCustomFormationWithEnemyType;

	// current enemy formation with enemy type
	std::vector<int> _currentFormation;
};

#endif // !_GAME_MANAGER_H_
