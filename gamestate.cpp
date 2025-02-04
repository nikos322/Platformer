#include "Gamestate.h"
#include "level.h"
#include "player.h"
#include <thread>
#include <chrono>
#include <string>
GameState::GameState()
{

}

GameState* GameState::getInstance()
{
	if (m_unique_instance == nullptr)
		m_unique_instance = new GameState();

	return m_unique_instance;
}

GameState::~GameState()
{
	if (m_player)
		delete m_player;
	if (m_currentlevel)
		delete m_currentlevel;
}

void GameState::init()
{
	m_current_stage = 0;
	if (m_current_stage <= 3 && m_current_stage >0)
	{
		m_player = new Player("player");
		m_player->init();
	}
	m_currentlevel = new Level();
	m_currentlevel->init();

	graphics::preloadBitmaps(getAssetDir());
	graphics::setFont(getFullAssetPath("font.ttf"));
}

void GameState::draw()
{
	if (!m_currentlevel)
		return;

	m_currentlevel->draw();
}

void GameState::update(float dt)
{
	// Skip an update if a long delay is detected
	// to avoid messing up the collision simulation	
	if (dt > 500) // ms
		return;
	float sleep_time = max(0.0f, 17.0f - dt);

	this_thread::sleep_for(chrono::duration<float, milli>(sleep_time));

	if (!m_currentlevel)
		return;
	m_currentlevel->update(dt);
	if (m_currentlevel->m_level_complete)
	{
		m_currentlevel->~Level();
		m_current_stage++;
		if (m_current_stage <= 3 && m_current_stage > 0)
		{
			m_player = new Player("player");
			m_player->init();
		}
		
		m_currentlevel = new Level();
		m_currentlevel->init();
	}

	m_debugging = graphics::getKeyState(graphics::SCANCODE_0);
}

string GameState::getAssetDir()
{
	return m_asset_path;
}

string GameState::getFullAssetPath(const string& asset)
{
	return m_asset_path + asset;
}

string GameState::getFullLevelPath()
{
	return  m_level_path + to_string(m_current_stage) + ".txt";
}
GameState* GameState::m_unique_instance = nullptr;