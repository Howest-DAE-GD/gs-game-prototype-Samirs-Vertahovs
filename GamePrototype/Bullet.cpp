#include "pch.h"
#include "Bullet.h"
#include <iostream>

Bullet::Bullet(float width, float height, const Rectf& fieldboundaries) :
	m_Position			{ },
	m_Width				{ width },
	m_Height			{ height },
	m_FieldBoundaries	{ fieldboundaries },
	m_IsActivated		{ false }
{

}
Bullet::~Bullet()
{

}

void Bullet::Update(float elapsedSec, std::vector<NPC*>& npcs)
{
	if (m_IsActivated)
	{
		m_Position.x += m_Velocity.x * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		CheckHit(npcs);

		CheckBoundaries();
	}
}

void Bullet::Shoot(const Point2f& pos, const Vector2f& velocity)
{
	if (m_IsActivated == false)
	{
		m_Position = pos;
		m_Velocity = velocity;
		m_IsActivated = true;
	}
}

void Bullet::Draw() const
{
	if (m_IsActivated)
	{
		SetColor(Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		FillEllipse(m_Position, m_Width, m_Height);
	}
}

void Bullet::CheckBoundaries()
{
	if (IsPointInRect(m_Position, m_FieldBoundaries) == false)
	{
		m_IsActivated = false;
	}
}

void Bullet::CheckHit(std::vector<NPC*>& npcs)
{
	for (NPC* npc : npcs)
	{
		if (npc->DoHitTest(Rectf{ m_Position.x, m_Position.y, m_Width, m_Height }))
		{
			m_IsActivated = false;
		}
	}
}