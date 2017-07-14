#pragma once
#include "XethCore.h"

namespace Xeth {
	class CPhysics
	{
	public:
		CPhysics(Point _gravity, Point _ground_pos, Point _ground_size);
		~CPhysics();

		b2Vec2* GetGravity();
		b2World* GetWorld();
		b2Body* GetGroundBody();

	private:
		b2Vec2* m_gravity;
		b2World* m_world;
		b2Body* m_ground_body;
	};
}