#include "XethPhysics.h"

#pragma comment(lib, "Box2D.lib")

using namespace Xeth;

CPhysics::CPhysics(Point _gravity, Point _ground_pos, Point _ground_size)
{
	m_gravity = new b2Vec2(_gravity.X, _gravity.Y);
	m_world = new b2World(*m_gravity);
	b2BodyDef bodyDef;
	bodyDef.position.Set(_ground_pos.X, _ground_pos.Y);
	m_ground_body = m_world->CreateBody(&bodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(_ground_size.X, _ground_size.Y);
	m_ground_body->CreateFixture(&groundBox, 0.f);
}


CPhysics::~CPhysics()
{
	SafeDelete(m_world);
	SafeDelete(m_gravity);
}

b2Vec2 * Xeth::CPhysics::GetGravity()
{
	return m_gravity;
}

b2World * Xeth::CPhysics::GetWorld()
{
	return m_world;
}

b2Body * Xeth::CPhysics::GetGroundBody()
{
	return m_ground_body;
}
