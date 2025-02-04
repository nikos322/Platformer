#pragma once
#include "gameobject.h"
#include "box.h"
#include "sgg/graphics.h"
using namespace std;


class Player : public GameObject,public Box
{
	graphics::Brush m_brush_player;
	vector<string> sprites;
	const float m_gravity = 10.0f;
	const float m_accel_vertical = 6.0f;
	const float m_accel_horizontal = 30.0f;
	const float m_max_velocity = 5.0f;
	void movePlayer(float dt);

public:
	float m_velocity_x = 0.0f;
	float m_velocity_y = 0.0f;
	float m_jumpcd = 5.0f;
	float m_health = 100.0f;

	Player(string name) : GameObject(name) {}
	void update(float dt) override;
	void init() override;
	void draw() override;

protected:
	void drawdebug();

};
