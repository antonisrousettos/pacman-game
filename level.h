#pragma once

#include "gameobject.h"
#include <vector>
#include <list>
#include <string>
#include <sgg/graphics.h>
#include "player.h"
#include "Collectible.h"

class Level : public GameObject
{
	graphics::Brush m_brush_background;

	typedef enum { STATUS_START, STATUS_PLAYING, GAME_OVER, GAME_WIN } status_t;
	status_t status = STATUS_START;

	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_dynamic_objects; 
	std::vector<Collectible*> m_collectibles; 
	int m_collected_count = 0; 

	// add some collidable blocks
	std::vector<Box> m_blocks;
	std::vector<std::string> m_block_names;
	const float m_block_size = 1.0f;
	graphics::Brush m_block_brush;

	graphics::Brush text;
	std::string timer = "100";
	int num;

	float m_center_x = 5.0f;
	float m_center_y = 5.0f;

	// dedicated method to draw a block
	void drawBlock(int i);

	void checkCollisions();
	void updateStartScreen(float dt);
	void updateLevelScreen(float dt);
	void updateVictoryScreen(float dt);
	void updateOverScreen(float dt);
	void drawStartScreen();
	void drawLevelScreen();
	void drawVictoryScreen();
	void drawOverScreen();
	void checkPlayerCollisions();
	void handlePlayerBlockCollision(Player* player, const Box& block, bool isPlayer1);
public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	int getCollectedCount() const { return m_collected_count; }

	Level(const std::string& name = "Level0");
	~Level() override;
};
