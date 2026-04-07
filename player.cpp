#include "player.h"
#include "util.h"
#include <cmath>

void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;

	movePlayer1(dt);

	movePlayer2(dt);

	GameObject::update(dt);

}

void Player::draw1()
{
	m_brush_player1.texture = m_sprites[current_direction];
	graphics::drawRect(m_pos_x1, m_pos_y1, 0.4f, 0.4f, m_brush_player1);

}

void Player::draw2()
{
	m_brush_player2.texture = m_state->getFullAssetPath("char_2.png");
	graphics::drawRect(m_pos_x2, m_pos_y2, 0.4f, 0.4f, m_brush_player2);

}

void Player::init()
{
	m_pos_x1 = 1.0f;
	m_pos_y1 = 9.0f;


	m_brush_player1.fill_opacity = 1.0f;
	m_brush_player1.outline_opacity = 0.0f;

	// Load sprite paths for each direction
	m_sprites["up"] = m_state->getFullAssetPath("char_1_up.png");
	m_sprites["down"] = m_state->getFullAssetPath("char_1_down.png");
	m_sprites["left"] = m_state->getFullAssetPath("char_1_left.png");
	m_sprites["right"] = m_state->getFullAssetPath("char_1_right.png");
	m_sprites["idle"] = m_state->getFullAssetPath("char_1_idle.png");

	m_pos_x2 = 5.0f;
	m_pos_y2 = 5.0f;

	m_brush_player2.fill_opacity = 1.0f;
	m_brush_player2.outline_opacity = 0.0f;

	// Adjust width for finer collision detection
	m_width = 0.4f;
	m_height = 0.4f; 
}


void Player::movePlayer1(float dt)
{
	float delta_time = dt / 1000.0f;

	move_speed = 5.0f;

	float move_x = 0.0f;
	float move_y = 0.0f;

	// Horizontal movement (A/D keys)
	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		move_x -= 5.0f;  
		current_direction = "left";
	}
	if (graphics::getKeyState(graphics::SCANCODE_D)) {
		move_x = 5.0f;   
		current_direction = "right";
	}

	// Vertical movement (W/S keys)
	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		move_y -= 5.0f;  
		current_direction = "up";
	}
	if (graphics::getKeyState(graphics::SCANCODE_S)) {
		move_y = 5.0f;   
		current_direction = "down";
	}

	// Default to idle if no key is pressed
	if (move_x == 0 && move_y == 0) {
		current_direction = "idle";
	}

	m_vx1 = std::min<float>(m_max_velocity, m_vx1 + delta_time * move_x * m_accel_horizontal);
	m_vx1 = std::max<float>(-m_max_velocity, m_vx1);

	m_vy1 = std::min<float>(m_max_velocity, m_vy1 + delta_time * move_y * m_accel_horizontal);
	m_vy1 = std::max<float>(-m_max_velocity, m_vy1);

	// Friction for smooth stopping
	m_vx1 -= 0.2f * m_vx1 / (0.1f + fabs(m_vx1));
	m_vy1 -= 0.2f * m_vy1 / (0.1f + fabs(m_vy1));

	// Static friction threshold to stop small movements
	if (fabs(m_vx1) < 0.01f)
		m_vx1 = 0.0f;
	if (fabs(m_vy1) < 0.01f)
		m_vy1 = 0.0f;

	m_pos_x1 += m_vx1 * delta_time;
	m_pos_y1 += m_vy1 * delta_time;
}


void Player::movePlayer2(float dt)
{
	float delta_time = dt / 1000.0f;
	move_speed = 5.0f;

	float move_x = 0.0f;
	float move_y = 0.0f;

	// Horizontal movement (A/D keys)
	if (graphics::getKeyState(graphics::SCANCODE_LEFT))
		move_x -= 8.0f;
	if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
		move_x = 8.0f;

	// Vertical movement (W/S keys)
	if (graphics::getKeyState(graphics::SCANCODE_UP))
		move_y -= 10.0f;  
	if (graphics::getKeyState(graphics::SCANCODE_DOWN))
		move_y = 10.0f;   

	// Update velocity
	m_vx2 = std::min<float>(m_max_velocity, m_vx2 + delta_time * move_x * m_accel_horizontal);
	m_vx2 = std::max<float>(-m_max_velocity, m_vx2);

	m_vy2 = std::min<float>(m_max_velocity, m_vy2 + delta_time * move_y * m_accel_horizontal);
	m_vy2 = std::max<float>(-m_max_velocity, m_vy2);

	// Friction for smooth stopping
	m_vx2 -= 0.2f * m_vx2 / (0.1f + fabs(m_vx2));
	m_vy2 -= 0.2f * m_vy2 / (0.1f + fabs(m_vy2));

	// Static friction threshold to stop small movements
	if (fabs(m_vx2) < 0.01f)
		m_vx2 = 0.0f;
	if (fabs(m_vy2) < 0.01f)
		m_vy2 = 0.0f;

	m_pos_x2 += m_vx2 * delta_time;
	m_pos_y2 += m_vy2 * delta_time;

}


