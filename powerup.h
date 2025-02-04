#pragma once
#include "gameobject.h"
#include "box.h"
#include <sgg/graphics.h>
using namespace std; 


class PowerUp : public GameObject, public Box
{
	graphics::Brush m_brush_powerup;

public:

	void init() override;
	void draw() override;
	void update(float dt) override;
	PowerUp(string name, float x, float y) :GameObject(name) 
	{
		m_pos_x = x;
		m_pos_y = y;
	};
	~PowerUp() {};
};
