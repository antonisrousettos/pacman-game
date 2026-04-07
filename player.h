#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"
#include <map>
#include <string>

class Player : public Box, public GameObject
{
	// animated player
	std::map<std::string, std::string> m_sprites;
	std::string current_direction = "idle";

	graphics::Brush m_brush_player1;
	graphics::Brush m_brush_player2;

	const float m_accel_horizontal = 20.0f;
	const float m_accel_vertical = 300.1f;
	const float m_max_velocity = 5.0f;
	const float m_gravity = 10.0f;
public:
	float m_vx1 = 0.0f;
	float m_vy1 = 0.0f;
	float m_vx2 = 0.0f;
	float m_vy2 = 0.0f;
	float move_speed = 0.0f;


public:
	void update(float dt) override;
	void draw1() override;
	void draw2() override;
	void init() override;
	Player(std::string name) : GameObject(name) {}


protected:

	// dynamic motion control
	void movePlayer1(float dt);
	void movePlayer2(float dt);
};
