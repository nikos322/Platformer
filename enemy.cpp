#include "enemy.h"
#include "Gamestate.h"
#include "player.h"
#include "util.h"
void Enemy::init()
{
	m_pos_y = floorf(m_state->getPlayer()->m_pos_y) + (rand() % 10) - 6.0f; // spawn the enemy up to 6 blocks away from the player 
	m_pos_x = floorf(m_state->getPlayer()->m_pos_x) - 13.0f;
	m_brush_enemy.fill_opacity = 1.0f;
	m_brush_enemy.outline_opacity = 0.0f;
	SETCOLOR(m_brushdebug_enemy.fill_color, 1, 0, 0);
	SETCOLOR(m_brushdebug_enemy.outline_color, 1, 0, 0);
	m_brushdebug_enemy.fill_opacity = 0.4f;
	m_brushdebug_enemy.outline_opacity = 1.0f;
	sprites.push_back(m_state->getFullAssetPath("monkey1.png"));
	sprites.push_back(m_state->getFullAssetPath("monkey2.png"));
	sprites.push_back(m_state->getFullAssetPath("monkey3.png"));
	sprites.push_back(m_state->getFullAssetPath("monkey4.png"));
	sprites.push_back(m_state->getFullAssetPath("monkey5.png"));
	sprites.push_back(m_state->getFullAssetPath("monkey6.png"));
	sprites.push_back(m_state->getFullAssetPath("monkey7.png"));
	sprites.push_back(m_state->getFullAssetPath("monkey8.png"));
}

void Enemy::draw()
{
	int s = (int)fmodf(1000 - m_pos_x * 2.0f, sprites.size());
	m_brush_enemy.texture = sprites[s];
	drawRect(m_pos_x + m_state->m_global_offset_x,
		m_pos_y + m_state->m_global_offset_y, 1.0f, 1.0f, m_brush_enemy);

	if (m_state->m_debugging) drawdebug();
}
void Enemy::update(float dt)
{
	float delta_time = dt / 1000.0f;
	m_pos_x += delta_time * m_velocity;
}
void Enemy::drawdebug()
{
	drawRect(m_pos_x + m_state->m_global_offset_x,
		m_pos_y + m_state->m_global_offset_y, 1.0f, 1.0f, m_brushdebug_enemy);
}
Enemy::~Enemy(){}