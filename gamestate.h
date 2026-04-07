#pragma once

#include <string>

class GameState
{
private:

	static GameState* m_unique_instance;

	const std::string m_asset_path = "assets\\";

	const float m_canvas_width = 10.0f;
	const float m_canvas_height = 10.0f;

	class Level* m_current_level = 0;
	class Player* m_player1 = 0;
	class Player* m_player2 = 0;
	



	GameState();

public:
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	int m_collected_total = 0;
	bool m_debugging = false;

public:
	~GameState();
	static GameState* getInstance();

	int getCollectedTotal() const { return m_collected_total; } 

	bool init();
	void draw();
	void update(float dt);

	std::string getFullAssetPath(const std::string& asset);
	std::string getAssetDir();


	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	class Player* getPlayer1() { return m_player1; }
	class Player* getPlayer2() { return m_player2; }
	class Level* getLevel() { return m_current_level; }
};
