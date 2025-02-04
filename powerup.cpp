#include "powerup.h"
#include "box.h"
#include "gamestate.h"
#include <sgg/graphics.h>

void PowerUp::init(){}

void PowerUp::draw()
{
	m_brush_powerup.fill_opacity = 1.0f;
	m_brush_powerup.outline_opacity = 0.0f;
	m_brush_powerup.texture = m_state->getFullAssetPath("banana.png");
	graphics::drawRect(m_pos_x + m_state->m_global_offset_x,
		m_pos_y + m_state->m_global_offset_y, m_width, m_width * 0.9f, m_brush_powerup);
}

void PowerUp::update(float dt){}