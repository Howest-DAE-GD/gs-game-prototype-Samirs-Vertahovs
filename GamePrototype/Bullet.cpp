#include "pch.h"
#include "Bullet.h"
#include "Player.h"
#include <iostream>

Bullet::Bullet(float width, float height, const Rectf& fieldboundaries, Player* player) :
	m_Position			{ },
	m_Width				{ width },
	m_Height			{ height },
	m_FieldBoundaries	{ fieldboundaries },
	m_IsActivated		{ false },
	m_Time				{ },
	m_PtrPlayer			{ player }
{

}

Bullet::Bullet(float width, float height, const Rectf& fieldboundaries) :
	m_Position			{ },
	m_Width				{ width },
	m_Height			{ height },
	m_FieldBoundaries	{ fieldboundaries },
	m_IsActivated		{ false },
	m_Time				{ },
	m_PtrPlayer			{ nullptr }
{

}

Bullet::~Bullet()
{

}

void Bullet::Update(float elapsedSec, std::vector<NPC*>& npcs, int& killedEnemies, int& killedCivs)
{
	if (m_IsActivated)
	{
		m_Position.x += m_Velocity.x * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		CheckHit(npcs, killedEnemies, killedCivs);

		CheckBoundaries();

		m_Time += elapsedSec;
	}

	if (m_Time >= 1.0f)
	{
		m_IsActivated = false;
		m_Time = 0.0f;
	}
}

void Bullet::Update(float elapsedSec)
{
	if (m_IsActivated)
	{
		m_Position.x += m_Velocity.x * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;

		if (m_PtrPlayer->DoHitTest(Rectf{ m_Position.x, m_Position.y, m_Width, m_Height }))
		{
			m_IsActivated = false;
			m_PtrPlayer->Die();
		}

		CheckBoundaries();

		m_Time += elapsedSec;
	}

	if (m_Time >= 1.8f)
	{
		m_IsActivated = false;
		m_Time = 0.0f;
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

void Bullet::CheckHit(std::vector<NPC*>& npcs, int& killedEnemies, int& killedCivs)
{
	for (NPC* npc : npcs)
	{
		if (npc->DoHitTest(Rectf{ m_Position.x, m_Position.y, m_Width, m_Height }))
		{
			if (npc->IsEnemy())
			{
				killedEnemies += 1;
			}
			else
			{
				killedCivs += 1;
			}
			m_IsActivated = false;
		}
	}

}