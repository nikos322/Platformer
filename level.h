#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"
#include <list>
#include "box.h"
#include "enemy.h"
#include "powerup.h"
class Level :public GameObject
{
	graphics::Brush m_brush_background;

	float m_center_x = 5.0f;
	float m_center_y = 5.0f;

	Box m_exit;
	graphics::Brush m_exit_brush;
	// Blocks


	vector<Box> m_blocks;
	vector<string> m_block_names;
	const float m_block_size = 1.0f;

	graphics::Brush m_blocks_brush;
	graphics::Brush m_blocks_brush_debug;

	void drawBlock(int i);

	void checkCollisions();

	//enemies

	int m_enemy_diff = 0;
	float m_enemy_spawn_rate;
	vector<Enemy> m_enemies;

	vector<PowerUp> m_powerups;

	graphics::Brush m_brush_healthbar;
	graphics::Brush m_brush_health;
public:
	bool m_level_complete = false;
	void update(float dt) override;
	void init() override;
	void draw() override;
	Level(const string& name = "Level 0");
	~Level() override;

};
