#include "player.h"
#include "sgg/graphics.h"
#include "gamestate.h"
#include "util.h"
void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	float move = 0.0f;

	if (graphics::getKeyState(graphics::SCANCODE_A))
		move -= 1.0f;
	if (graphics::getKeyState(graphics::SCANCODE_D))
		move += 1.0f;

	m_velocity_x = min(m_max_velocity ,m_velocity_x + delta_time * move * m_accel_horizontal);
	m_velocity_x = max(-m_max_velocity, m_velocity_x);
	m_velocity_x -= 0.2f * m_velocity_x/(0.1f + fabs(m_velocity_x));
	if (fabs(m_velocity_x) < 0.01f)
		m_velocity_x = 0.0f;
	m_pos_x += delta_time * m_velocity_x;
	m_jumpcd--;

	if (m_velocity_y == 0.0f && m_jumpcd <= 0.0f)
	{
		m_velocity_y -= graphics::getKeyState(graphics::SCANCODE_W) ? m_accel_vertical : 0.0f;
		if (graphics::getKeyState(graphics::SCANCODE_W)) m_jumpcd = 5.0f;
	}
	m_velocity_y += delta_time * m_gravity;

	m_pos_y += m_velocity_y * delta_time;

}
void Player::update(float dt)
{
	movePlayer(dt);

	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

}

void Player::init()
{
	m_pos_x = 0.0f;
	m_pos_y = 0.0f;
	m_width /= 1.9f;
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;
	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	sprites.push_back(m_state->getFullAssetPath("tarzan1.png"));
	sprites.push_back(m_state->getFullAssetPath("tarzan2.png"));
	sprites.push_back(m_state->getFullAssetPath("tarzan3.png"));
}

void Player::draw()
{
	int s = (int)fmodf(1000.0f - m_pos_x * 2.0f, sprites.size());
	m_brush_player.texture = sprites[s];
	if (m_velocity_x < 0)
		graphics::setScale(-1.0f, 1.0f);
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f,
						m_state->getCanvasHeight() * 0.5f,
						0.7f, 1.0f, m_brush_player);
	graphics::resetPose();
	if (m_state->m_debugging)
		drawdebug();
	
}

void Player::drawdebug()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f,
			m_state->getCanvasHeight() * 0.5f, m_width, m_height, debug_brush);
	
	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f,
			m_state->getCanvasHeight() * 0.5f - 0.6f, 0.15f, s, debug_brush);
}
