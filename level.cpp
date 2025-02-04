#include "level.h"
#include "Gamestate.h"
#include "player.h"
#include <sgg/graphics.h>
#include "util.h"
#include <fstream>
#include <iostream>    
#include <sstream>  
#include <string>
void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;
	m_blocks_brush.texture = m_state->getFullAssetPath(m_block_names[i]);

	graphics::drawRect(x, y,1.2f * m_block_size,1.2f * m_block_size,m_blocks_brush);

	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_blocks_brush_debug);
}
void Level::checkCollisions()
{
	//block collisions
	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(box))
		{
			m_state->getPlayer()->m_pos_x += offset;
			m_state->getPlayer()->m_velocity_x = 0.0f;
			break;
		}
	}
	for (auto& box : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(box))
		{
			if (fabs(offset) < 0.2f)
			{
				m_state->getPlayer()->m_pos_y += offset;
				m_state->getPlayer()->m_velocity_y = 0.0f;
			}
			break;
		}
	}
	for (auto& box : m_blocks)
	{
		float offset;
		if (offset = m_state->getPlayer()->intersectUp(box))
		{
			if (fabs(offset) < 0.2f)
			{
				m_state->getPlayer()->m_pos_y -= offset;
				m_state->getPlayer()->m_velocity_y = 0.0f;
			}
			break;
		}
	}
	

	// enemy collisions
	// enemy kill check
	for (int i = 0; i < m_enemies.size(); i++)
	{
		float offset;
		if (offset = m_state->getPlayer()->intersectDown(m_enemies[i]))
		{
			if (fabs(offset) < 0.3f)
			{
				playSound(m_state->getFullAssetPath("death.mp3"), 0.9, false);
				m_enemies.erase(m_enemies.begin() + i);
				i--;
			}
		}
	}
	// enemy damages player
	for (Enemy enemy : m_enemies)
	{
		if (m_state->getPlayer()->intersect(enemy))
		{
			playSound(m_state->getFullAssetPath("hit.wav"), 0.1, false);
			m_state->getPlayer()->m_health -= 0.2f * m_enemy_diff;
		}
	}
	// power collection
	for (int i = 0; i < m_powerups.size(); i++)
	{
		if (m_state->getPlayer()->intersect(m_powerups[i]))
		{
			m_state->getPlayer()->m_health = 100.0f;
			m_powerups.erase(m_powerups.begin() + i);
			i--;
			graphics::playSound(m_state->getFullAssetPath("powerup.wav"), 0.5, false);
		}
	}
	if (m_state->getPlayer()->intersect(m_exit))
	{
		m_level_complete = true;
		graphics::playSound(m_state->getFullAssetPath("finish.mp3"), 0.2, false);
	}
}
void Level::update(float dt)
{
	switch (m_state->m_current_stage)
	{
	case 0:
		if (graphics::getKeyState(graphics::SCANCODE_SPACE)) m_level_complete = true;
		break;
	case 4:
		break;
	case -1:
		break;
	default:
		if (m_state->getPlayer()->isActive())
			m_state->getPlayer()->update(dt);
		//check player health for death
		if (m_state->getPlayer()->m_health <= 0.0f)
		{
			m_state->m_current_stage = -2;
			m_level_complete = true;
		}
		//spawn enemy
		if (m_enemy_spawn_rate <= 0.0f && m_enemies.size() < 25)
		{
			Enemy enemy = Enemy("monke", m_enemy_diff);
			enemy.init();
			m_enemies.push_back(enemy);
			m_enemy_spawn_rate = 1000.0f / (m_enemy_diff + 1);
		}
		m_enemy_spawn_rate -= 10.0f / m_enemy_diff;

		for (int i = 0; i < m_enemies.size(); i++)
		{
			m_enemies[i].update(dt);
			if (m_enemies[i].m_pos_x > m_state->getPlayer()->m_pos_x + 7)
			{
				m_enemies.erase(m_enemies.begin() + i);
				i--;
			}
		}
		checkCollisions();
	}

	GameObject::update(dt);
}

void Level::draw()
{
	switch (m_state->m_current_stage)
	{
	case -1:
	case 0:
	case 4:
		graphics::drawRect(m_state->getCanvasWidth() / 2, m_state->getCanvasHeight() / 2,
			m_state->getCanvasWidth(), m_state->getCanvasHeight(), m_brush_background);
		break;
	default:

		float w = m_state->getCanvasWidth() * 1.6f;
		float h = m_state->getCanvasHeight() * 1.6f;

		float offset_x = m_state->m_global_offset_x / 2.0f + w / 2.0f;
		float offset_y = m_state->m_global_offset_y / 2.0f + h / 2.0f;

		//draw background
		graphics::drawRect(offset_x, offset_y, 2.0f * w, 2.0f * h,
			m_brush_background);

		if (m_state->getPlayer()->isActive())
			m_state->getPlayer()->draw();

		for (int i = 0; i < m_blocks.size(); i++)
		{
			drawBlock(i);
		}
		//exit
		graphics::drawRect(m_exit.m_pos_x + m_state->m_global_offset_x,
			m_exit.m_pos_y + m_state->m_global_offset_y, m_exit.m_width, m_exit.m_height,
			m_exit_brush);
		//draw enemies
		for (Enemy enemy : m_enemies)
		{
			enemy.draw();
		}
		//draw power ups
		for (PowerUp power : m_powerups)
		{
			power.draw();
		}
		if (m_state->getPlayer()->isActive())
		{
			graphics::drawRect(m_state->getCanvasWidth() * 0.85f, m_state->getCanvasHeight() * 0.07f,
				m_state->getCanvasWidth() * 0.2f, m_state->getCanvasHeight() * 0.025f,
				m_brush_healthbar);
			graphics::drawRect(8.5 - (1 - m_state->getPlayer()->m_health / 100), 0.56,
				2 * m_state->getPlayer()->m_health / 100, 0.19, m_brush_health);
			break;
		}
	}
}

void Level::init()
{
	m_brush_background.outline_opacity = 0.0f;
	
	switch (m_state->m_current_stage)
	{
	case -1:
		graphics::stopMusic();
		m_brush_background.texture = m_state->getFullAssetPath("lose.png");
		break;
	case 0:
		graphics::playMusic(m_state->getFullAssetPath("music.mp3"),0.5,true);
		m_brush_background.texture = m_state->getFullAssetPath("start.png");
		break;
	case 4:
		graphics::stopMusic();
		graphics::playSound(m_state->getFullAssetPath("finish.mp3"), 0.2, true);
		m_brush_background.texture = m_state->getFullAssetPath("ending.png");
		break;
	default:
		m_brush_background.texture = m_state->getFullAssetPath("background.png");

		ifstream f (m_state->getFullLevelPath());
		string line;
		char delim = '/';
		string token;
		while (getline(f, line))
		{
			stringstream readline(line);
			getline(readline, token, delim);
			if (token == "block")
			{
				getline(readline, token, delim);
				float x = stof(token);
				getline(readline, token, delim);
				float y = stof(token);
				m_blocks.push_back(Box(x * m_block_size, y * m_block_size, m_block_size, m_block_size));
				getline(readline, token, delim);
				m_block_names.push_back(token);
			}
			else if (token == "enemy")
			{
				getline(readline, token, delim);
				m_enemy_diff = stoi(token);
			}
			else if (token == "powerup")
			{
				getline(readline, token, delim);
				float x = stof(token);
				getline(readline, token, delim);
				float y = stof(token);
				m_powerups.push_back(PowerUp("health", x, y));
			}
			else if (token == "exit")
			{
				getline(readline, token, delim);
				float x = stof(token);
				getline(readline, token, delim);
				float y = stof(token);
				m_exit_brush.fill_opacity = 1.0f;
				m_exit_brush.outline_opacity = 0.0f;
				m_exit_brush.texture = m_state->getFullAssetPath("portal.png");
				m_exit = Box(x, y, m_block_size, m_block_size);
			}
			else if (token == "start")
			{
				getline(readline, token, delim);
				float x = stof(token);
				getline(readline, token, delim);
				float y = stof(token);
				m_state->getPlayer()->m_pos_x = x;
				m_state->getPlayer()->m_pos_y = y;

			}
		}
		m_brush_healthbar.fill_opacity = 0.0f;
		m_brush_healthbar.outline_opacity = 1.0f;
		SETCOLOR(m_brush_healthbar.outline_color, 1, 1, 1);
		m_brush_health.fill_opacity = 1.0f;
		m_brush_health.outline_opacity = 0.0f;
		SETCOLOR(m_brush_health.fill_color, 0, 1, 0);

		for (PowerUp power : m_powerups)
		{
			power.init();
		}

		f.close();
	}
	

	m_blocks_brush.outline_opacity = 0.0f; 
	m_blocks_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_blocks_brush_debug.fill_color, 0.2f, 1.0f, 0.1f);
	SETCOLOR(m_blocks_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);

}

Level::Level(const string& name){}

Level::~Level()
{
	delete m_state->getPlayer();
	m_enemies.clear();
	m_blocks.clear();
	m_powerups.clear();
	m_block_names.clear();
}


