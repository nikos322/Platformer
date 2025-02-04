#pragma once
#include <string>
#include "sgg/graphics.h"
using namespace std;
class GameState
{
private:
	string m_asset_path = "assets\\";
	string m_level_path = "Levels\\";
	float m_canvas_width = 10.0f;
	float m_canvas_height = 8.0f;

	static GameState* m_unique_instance;

	GameState();
	
	class Player* m_player = 0;
	class Level* m_currentlevel = 0;

public:
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	bool m_debugging = false;
	int m_current_stage;

	static GameState* getInstance();
	~GameState();
	void init();
	void draw();
	void update(float dt);

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	string getAssetDir();
	string getFullAssetPath(const string& asset);
	string getFullLevelPath();
	class Player* getPlayer() { return m_player; }
};
