#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "util.h"
#include "box.h"
#include <sstream>

void Level::drawStartScreen()
{
	graphics::Brush startscreen;
	startscreen.outline_opacity = 0.0f;
	startscreen.texture = m_state->getFullAssetPath("start.png");
	graphics::drawRect(5.0f, 5.0f, 10.0f, 10.0f, startscreen);
}

void Level::checkPlayerCollisions() {
	Player* player1 = m_state->getPlayer1();
	Player* player2 = m_state->getPlayer2();

	if (fabs(player1->m_pos_x1 - player2->m_pos_x2) < (player1->m_width / 2 + player2->m_width / 2) &&
		fabs(player1->m_pos_y1 - player2->m_pos_y2) < (player1->m_height / 2 + player2->m_height / 2)) {
		status = GAME_OVER; // Set game over status when players collide
	}
}

void Level::drawLevelScreen()
{
	// Background drawing
	float w = 10.0f;
	float h = 10.0f;

	graphics::drawRect(5.0f, 5.0f, w, h, m_brush_background);


	if (m_state->getPlayer1()->isActive()) {
		m_state->getPlayer1()->draw1();
	}

	if (m_state->getPlayer2()->isActive()) {
		m_state->getPlayer2()->draw2();
	}

	// Draw blocks (collision only)
	for (auto& block : m_blocks) {
		graphics::Brush invisible_block;
		invisible_block.outline_opacity = 0.0f; 
		invisible_block.fill_opacity = 0.0f; 
		graphics::drawRect(block.m_pos_x1, block.m_pos_y1, block.m_width, block.m_height, invisible_block);
	}


	// Debug
	//for (int i = 0; i < m_blocks.size(); i++) {
	//	drawBlock(i);
	//}

	// Draw collectibles
	for (auto& collectible : m_collectibles) {
		if (collectible && !collectible->isCollected()) {
			collectible->draw();
		}
	}

	
}


void Level::drawVictoryScreen() {
	graphics::Brush victoryscreen;
	victoryscreen.outline_opacity = 0.0f;
	victoryscreen.texture = m_state->getFullAssetPath("win.png");
	graphics::drawRect(5.0f, 5.0f, 10.0f, 10.0f, victoryscreen);

	// Draw the number of collectibles collected
	graphics::Brush text_brush;
	text_brush.fill_color[0] = 1.0f;
	text_brush.fill_color[1] = 1.0f;
	text_brush.fill_color[2] = 1.0f;
	text_brush.outline_opacity = 0.0f;

	std::ostringstream collectible_count_text;
	collectible_count_text << "Total Collectibles: " << m_collected_count;
	graphics::drawText(1.0f, 9.0f, 0.5f, collectible_count_text.str(), text_brush);
}

void Level::drawOverScreen()
{
	graphics::Brush overscreen;
	overscreen.outline_opacity = 0.0f;
	overscreen.texture = m_state->getFullAssetPath("over.png");
	graphics::drawRect(5.0f, 5.0f, 10.0f, 10.0f, overscreen);
}


void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];

	float x1 = box.m_pos_x1;
	float y1 = box.m_pos_y1;
	float w = box.m_width;
	float h = box.m_height;

	m_block_brush.fill_color[2];
	m_block_brush.fill_opacity = 1.0f;

	graphics::drawRect(x1, y1, w, h, m_block_brush);


}

void Level::checkCollisions() {
	Player* player1 = m_state->getPlayer1();
	Player* player2 = m_state->getPlayer2();

	for (auto& block : m_blocks) {
		// Collision handling for Player 1
		handlePlayerBlockCollision(player1, block, true);

		// Collision handling for Player 2
		handlePlayerBlockCollision(player2, block, false);
	}
}

void Level::handlePlayerBlockCollision(Player* player, const Box& block, bool isPlayer1) {
	float& player_pos_x = isPlayer1 ? player->m_pos_x1 : player->m_pos_x2;
	float& player_pos_y = isPlayer1 ? player->m_pos_y1 : player->m_pos_y2;
	float& player_vx = isPlayer1 ? player->m_vx1 : player->m_vx2;
	float& player_vy = isPlayer1 ? player->m_vy1 : player->m_vy2;

	float player_half_width = player->m_width / 2.0f;
	float player_half_height = player->m_height / 2.0f;
	float block_half_width = block.m_width / 2.0f;
	float block_half_height = block.m_height / 2.0f;

	float dx = fabs(player_pos_x - block.m_pos_x1);
	float dy = fabs(player_pos_y - block.m_pos_y1);

	if (dx < (player_half_width + block_half_width) &&
		dy < (player_half_height + block_half_height)) {

		float overlap_x = (player_half_width + block_half_width) - dx;
		float overlap_y = (player_half_height + block_half_height) - dy;

		if (overlap_x < overlap_y) {
			if (player_pos_x < block.m_pos_x1) {
				player_pos_x -= overlap_x; 
			}
			else {
				player_pos_x += overlap_x; 
			}
			player_vx = 0.0f; 
		}
		else {
			if (player_pos_y < block.m_pos_y1) {
				player_pos_y -= overlap_y; 
			}
			else {
				player_pos_y += overlap_y; 
			}
			player_vy = 0.0f; 
		}
	}
}






void Level::updateStartScreen(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		status = STATUS_PLAYING;
	}
}

void Level::updateLevelScreen(float dt)
{
	if (m_state->getPlayer1()->isActive())
		m_state->getPlayer1()->update(dt);

	if (m_state->getPlayer2()->isActive())
		m_state->getPlayer2()->update(dt);

	// Check for collectible collisions
	for (auto& collectible : m_collectibles) {
		if (collectible && !collectible->isCollected()) {
			if (collectible->checkCollision(m_state->getPlayer1())){
				m_collected_count++;
			}
		}

		if (m_collected_count == m_collectibles.size())
		{
			status = GAME_WIN;
		}

	}

	checkCollisions(); 
	checkPlayerCollisions();


	GameObject::update(dt);
}

void Level::updateVictoryScreen(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		graphics::destroyWindow();
	}
}

void Level::updateOverScreen(float dt)
{
	if (graphics::getKeyState(graphics::SCANCODE_SPACE))
	{
		graphics::destroyWindow();

	}
}

void Level::update(float dt)
{
	if (status == STATUS_START)
	{
		updateStartScreen(dt);
	}
	else if (status == STATUS_PLAYING)
	{
		updateLevelScreen(dt);
	}
	else if (status == GAME_WIN)
	{
		updateVictoryScreen(dt);
	}
	else if (status == GAME_OVER)
	{
		updateOverScreen(dt);
	}
}

void Level::draw()
{
	if (status == STATUS_START)
	{
		drawStartScreen();
	}
	else if (status == STATUS_PLAYING)
	{
		drawLevelScreen();
	}
	else if (status == GAME_WIN)
	{
		drawVictoryScreen();
	}
	else if (status == GAME_OVER)
	{
		drawOverScreen();
	}
}


void Level::init()
{


	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

	// Initialize collectibles
	m_collectibles.push_back(new Collectible(2.0f, 2.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(4.0f, 3.7f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(6.0f, 2.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(8.0f, 2.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(5.7f, 3.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(5.7f, 3.7f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(2.0f, 3.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(3.4f, 5.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(6.4f, 5.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(3.4f, 6.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(6.4f, 6.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(3.4f, 6.7f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(6.4f, 6.7f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(3.4f, 7.6f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(6.4f, 7.6f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(2.4f, 5.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(7.6f, 5.0f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(2.4f, 9.3f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(8.4f, 9.3f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(3.4f, 9.3f, 0.3f, m_state->getFullAssetPath("collectible.png")));
	m_collectibles.push_back(new Collectible(6.4f, 9.3f, 0.3f, m_state->getFullAssetPath("collectible.png")));

	// Initialize collectible counter
	m_collected_count = 0;

	//left
	m_blocks.push_back(Box(0.1, 0, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 0.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 0.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 0.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 1, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 1.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 1.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 1.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 2, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 2.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 2.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 2.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 3, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 3.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 3.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 3.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 4, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 4.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 4.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 4.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 5.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 5.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 5.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 6, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 6.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 6.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 6.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 7, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 7.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 7.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 7.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 8, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 8.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 8.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 8.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 9, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 9.25, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 9.5, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 9.75, 0.25, 0.25));
	m_blocks.push_back(Box(0.1, 10, 0.25, 0.25));
	//right
	m_blocks.push_back(Box(9.8, 0, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 0.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 0.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 0.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 1, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 1.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 1.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 1.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 2, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 2.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 2.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 2.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 3, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 3.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 3.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 3.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 4, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 4.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 4.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 4.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 5.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 5.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 5.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 6, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 6.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 6.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 6.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 7, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 7.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 7.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 7.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 8, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 8.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 8.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 8.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 9, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 9.25, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 9.5, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 9.75, 0.25, 0.25));
	m_blocks.push_back(Box(9.8, 10, 0.25, 0.25));

	//bottom
	m_blocks.push_back(Box(0, 10, 0.25, 0.25));
	m_blocks.push_back(Box(0.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(0.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(0.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(1, 10, 0.25, 0.25));
	m_blocks.push_back(Box(1.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(1.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(1.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(2, 10, 0.25, 0.25));
	m_blocks.push_back(Box(2.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(2.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(2.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(3, 10, 0.25, 0.25));
	m_blocks.push_back(Box(3.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(3.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(3.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(4, 10, 0.25, 0.25));
	m_blocks.push_back(Box(4.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(4.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(4.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(5.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(5.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(5.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(6, 10, 0.25, 0.25));
	m_blocks.push_back(Box(6.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(6.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(6.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(7, 10, 0.25, 0.25));
	m_blocks.push_back(Box(7.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(7.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(7.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(8, 10, 0.25, 0.25));
	m_blocks.push_back(Box(8.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(8.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(8.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(9, 10, 0.25, 0.25));
	m_blocks.push_back(Box(9.25, 10, 0.25, 0.25));
	m_blocks.push_back(Box(9.5, 10, 0.25, 0.25));
	m_blocks.push_back(Box(9.75, 10, 0.25, 0.25));
	m_blocks.push_back(Box(10, 10, 0.25, 0.25));

	//top
	m_blocks.push_back(Box(0, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(0.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(0.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(0.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(1, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(1.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(1.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(1.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(2, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(2.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(2.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(2.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(3, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(3.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(3.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(3.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(4, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(4.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(4.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(4.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(5.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(5.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(5.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(6, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(6.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(6.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(6.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(7, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(7.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(7.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(7.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(8, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(8.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(8.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(8.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(9, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(9.25, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(9.5, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(9.75, 0.1, 0.25, 0.25));
	m_blocks.push_back(Box(10, 0.1, 0.25, 0.25));

	//first floor
	m_blocks.push_back(Box(1.6, 1.3, 1, 0.5));
	m_blocks.push_back(Box(3.4, 1.3, 1.3, 0.5));
	m_blocks.push_back(Box(6.6, 1.3, 1.3, 0.5));
	m_blocks.push_back(Box(8.5, 1.3, 1, 0.5));

	m_blocks.push_back(Box(5, 0.25, 0.25, 0.25));
	m_blocks.push_back(Box(5, 0.50, 0.25, 0.25));
	m_blocks.push_back(Box(5, 0.75, 0.25, 0.25));
	m_blocks.push_back(Box(5, 1.0, 0.25, 0.25));
	m_blocks.push_back(Box(5, 1.25, 0.25, 0.25));
	m_blocks.push_back(Box(5, 1.50, 0.25, 0.25));

	

	//second floor 
	m_blocks.push_back(Box(1.6, 2.40, 1.00, 0.25));
	m_blocks.push_back(Box(3.0, 2.40, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 2.60, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 2.80, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 3.00, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 3.20, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 3.40, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 3.60, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 3.80, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 4.00, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 4.20, 0.25, 0.25));
	
	m_blocks.push_back(Box(5.0, 2.4, 2.40, 0.25));

	m_blocks.push_back(Box(3.6, 3.30, 1.20, 0.30));





	
	m_blocks.push_back(Box(5.0, 2.60, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 2.80, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 3.00, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 3.20, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 3.40, 0.25, 0.25));

	m_blocks.push_back(Box(7.1, 2.40, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 2.60, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 2.80, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 3.00, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 3.20, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 3.40, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 3.60, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 3.80, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 4.00, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 4.20, 0.25, 0.25));

	m_blocks.push_back(Box(6.6, 3.3, 1.40, 0.25));
	
	m_blocks.push_back(Box(8.5, 2.3, 1.10, 0.25));

	m_blocks.push_back(Box(1.0, 4.20, 1.70, 0.25));
	m_blocks.push_back(Box(1.0, 3.20, 1.70, 0.25));

	m_blocks.push_back(Box(1.9, 3.20, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 3.40, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 3.60, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 3.80, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 4.00, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 4.20, 0.25, 0.25));

	m_blocks.push_back(Box(8.8, 4.20, 1.70, 0.25));
	m_blocks.push_back(Box(8.8, 3.20, 1.70, 0.25));

	m_blocks.push_back(Box(8.1, 3.40, 0.25, 0.25));
	m_blocks.push_back(Box(8.1, 3.60, 0.25, 0.25));
	m_blocks.push_back(Box(8.1, 3.80, 0.25, 0.25));
	m_blocks.push_back(Box(8.1, 4.00, 0.25, 0.25));
	m_blocks.push_back(Box(8.1, 4.20, 0.25, 0.25));

	m_blocks.push_back(Box(8.8, 6.20, 1.70, 0.25));
	m_blocks.push_back(Box(8.8, 5.20, 1.70, 0.25));

	m_blocks.push_back(Box(8.0, 5.20, 0.25, 0.25));
	m_blocks.push_back(Box(8.0, 5.40, 0.25, 0.25));
	m_blocks.push_back(Box(8.0, 5.60, 0.25, 0.25));
	m_blocks.push_back(Box(8.0, 5.80, 0.25, 0.25));
	m_blocks.push_back(Box(8.0, 6.00, 0.25, 0.25));
	m_blocks.push_back(Box(8.0, 6.20, 0.25, 0.25));


	m_blocks.push_back(Box(1.0, 6.20, 1.70, 0.25));
	m_blocks.push_back(Box(1.0, 5.20, 1.70, 0.25));

	m_blocks.push_back(Box(1.9, 5.20, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 5.40, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 5.60, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 5.80, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 6.00, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 6.20, 0.25, 0.25));

	m_blocks.push_back(Box(2.9, 5.20, 0.25, 0.25));
	m_blocks.push_back(Box(2.9, 5.40, 0.25, 0.25));
	m_blocks.push_back(Box(2.9, 5.60, 0.25, 0.25));
	m_blocks.push_back(Box(2.9, 5.80, 0.25, 0.25));
	m_blocks.push_back(Box(2.9, 6.00, 0.25, 0.25));
	m_blocks.push_back(Box(2.9, 6.20, 0.25, 0.25));

	m_blocks.push_back(Box(7.1, 5.20, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 5.40, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 5.60, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 5.80, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 6.00, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 6.20, 0.25, 0.25));

	m_blocks.push_back(Box(5.0, 5.20, 2.40, 0.25));
	m_blocks.push_back(Box(4.1, 4.20, 0.7, 0.25));
	m_blocks.push_back(Box(5.9, 4.20, 0.7, 0.25));

	m_blocks.push_back(Box(3.9, 4.20, 0.25, 0.25));
	m_blocks.push_back(Box(3.9, 4.40, 0.25, 0.25));
	m_blocks.push_back(Box(3.9, 4.60, 0.25, 0.25));
	m_blocks.push_back(Box(3.9, 4.80, 0.25, 0.25));
	m_blocks.push_back(Box(3.9, 5.00, 0.25, 0.25));
	m_blocks.push_back(Box(3.9, 5.20, 0.25, 0.25));

	m_blocks.push_back(Box(6.1, 4.20, 0.25, 0.25));
	m_blocks.push_back(Box(6.1, 4.40, 0.25, 0.25));
	m_blocks.push_back(Box(6.1, 4.60, 0.25, 0.25));
	m_blocks.push_back(Box(6.1, 4.80, 0.25, 0.25));
	m_blocks.push_back(Box(6.1, 5.00, 0.25, 0.25));
	m_blocks.push_back(Box(6.1, 5.20, 0.25, 0.25));

	m_blocks.push_back(Box(5.0, 6.10, 2.40, 0.25));

	m_blocks.push_back(Box(5.0, 6.20, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 6.40, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 6.60, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 6.80, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 7.00, 0.25, 0.25));

	m_blocks.push_back(Box(3.4, 7.10, 1.20, 0.25));
	m_blocks.push_back(Box(6.6, 7.10, 1.20, 0.25));

	m_blocks.push_back(Box(1.5, 7.10, 0.90, 0.25));

	m_blocks.push_back(Box(8.5, 7.10, 0.90, 0.25));

	m_blocks.push_back(Box(1.9, 7.20, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 7.40, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 7.60, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 7.80, 0.25, 0.25));
	m_blocks.push_back(Box(1.9, 8.00, 0.25, 0.25));

	m_blocks.push_back(Box(8.1, 7.20, 0.25, 0.25));
	m_blocks.push_back(Box(8.1, 7.40, 0.25, 0.25));
	m_blocks.push_back(Box(8.1, 7.60, 0.25, 0.25));
	m_blocks.push_back(Box(8.1, 7.80, 0.25, 0.25));
	m_blocks.push_back(Box(8.1, 8.00, 0.25, 0.25));

	m_blocks.push_back(Box(5.0, 8.00, 2.30, 0.25));

	m_blocks.push_back(Box(5.0, 8.20, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 8.40, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 8.60, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 8.80, 0.25, 0.25));
	m_blocks.push_back(Box(5.0, 9.00, 0.25, 0.25));

	m_blocks.push_back(Box(3.0, 8.00, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 8.20, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 8.40, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 8.60, 0.25, 0.25));
	m_blocks.push_back(Box(3.0, 8.80, 0.25, 0.25));
	//m_blocks.push_back(Box(3.0, 9.00, 0.25, 0.25));

	m_blocks.push_back(Box(7.1, 8.00, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 8.20, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 8.40, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 8.60, 0.25, 0.25));
	m_blocks.push_back(Box(7.1, 8.80, 0.25, 0.25));
	//m_blocks.push_back(Box(7.1, 9.00, 0.25, 0.25));

	m_blocks.push_back(Box(2.7, 8.90, 3.00, 0.25));

	m_blocks.push_back(Box(7.4, 8.90, 3.00, 0.25));

	m_blocks.push_back(Box(0.7, 8.00, 0.50, 0.25));
	m_blocks.push_back(Box(9.3, 8.00, 0.40, 0.25));

	

}

Level::Level(const std::string& name)
	: GameObject(name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("bg_image.png");


}

Level::~Level()
{
	for (auto p_go : m_static_objects)
		delete p_go;
	for (auto p_go : m_dynamic_objects)
		delete p_go;

	for (auto& collectible : m_collectibles) {
		delete collectible;
	}
	m_collectibles.clear();
}

