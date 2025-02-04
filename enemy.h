#pragma once
#include "gameobject.h"
#include "box.h"
#include "sgg/graphics.h"
using namespace std;
using namespace graphics;

class Enemy : public GameObject, public Box
{
public:
	Brush m_brush_enemy;
	Brush m_brushdebug_enemy;
	float m_velocity;
	float m_damage;
	int m_difficulty;
	vector<string> sprites;

	Enemy(string name, int difficulty) : GameObject(name)
	{
		m_velocity = 2.0f + difficulty * 0.2f;
		m_damage = 1.0f * difficulty * 0.5f;
		m_difficulty = difficulty;
	}
	~Enemy();
	void init() override;
	void draw() override;
	void update(float dt) override;
	void drawdebug();
};
