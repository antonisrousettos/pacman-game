#pragma once
#include <string>
#include "gamestate.h"

class GameObject
{
	static int m_next_id;

protected:

	std::string m_name;
	int m_id = 0;
	bool m_active = true;

public:
	class GameState* m_state;
	GameObject(const std::string& name = "");
	virtual void update(float dt) {}
	virtual void init() {}
	virtual void draw() {}
	virtual void draw1() {}
	virtual void draw2() {}
	virtual ~GameObject() {}
	bool isActive() { return m_active; }
	void setActive(bool a) { m_active = a; }
};
