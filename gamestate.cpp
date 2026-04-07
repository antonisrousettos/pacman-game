#include "gamestate.h"
#include "gameobject.h"
#include "level.h"
#include "player.h"
#include <thread>
#include <chrono>
#include <sgg/graphics.h>
#include <iostream>
#include <filesystem>
#include <sstream>



using namespace std::chrono_literals;

GameState::GameState()
{
}

GameState::~GameState() {
	if (m_current_level) {
		delete m_current_level;
		m_current_level = nullptr;
	}

	if (m_player1) {
		delete m_player1;
		m_player1 = nullptr;
	}

	if (m_player2) {
		delete m_player2;
		m_player2 = nullptr;
	}
}

GameState* GameState::getInstance()
{
	if (!m_unique_instance)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}

bool GameState::init()
{


	m_current_level = new Level("Level 1");
	m_current_level->init();

	m_player1 = new Player("Player1");
	m_player1->init();

	m_player2 = new Player("Player2");
	m_player2->init();


	std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

	if (!std::filesystem::exists("equalar.ttf")) {
		std::cerr << "File equalar.ttf not found!" << std::endl;
	}

	graphics::preloadBitmaps(getAssetDir());
	graphics::setFont(m_asset_path + "OpenSans-Regular.ttf");

	return true;
}

void GameState::draw()
{
	if (!m_current_level)
		return;

	m_current_level->draw();
}

void GameState::update(float dt)
{
	// Skip an update if a long delay is detected 
	// to avoid messing up the collision simulation
	if (dt > 500) 
		return;

	// Avoid too quick updates
	float sleep_time = std::max(17.0f - dt, 0.0f);
	if (sleep_time > 0.0f)
	{
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));
	}

	if (!m_current_level)
		return;

	m_current_level->update(dt);

	// Sync the collectible counter from the level
	m_collected_total = m_current_level->getCollectedCount();

}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}


GameState* GameState::m_unique_instance = nullptr;