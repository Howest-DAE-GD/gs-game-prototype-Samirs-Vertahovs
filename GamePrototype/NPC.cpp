#include "pch.h"
#include "NPC.h"
#include "Bullet.h"
#include <iostream>

NPC::NPC(const Point2f& pos, float width, float height, bool enemy, const Rectf& fieldboundaries, Player* player) :
	m_Position{ pos },
	m_Width{ width },
	m_FieldBoundaries{ fieldboundaries },
	m_Height{ height },
	m_IsDead{ false },
	m_IsEnemy{ enemy },
	m_Bounds { },
	m_PtrPlayer{ player }
{
	m_Bounds.left = m_Position.x;
	m_Bounds.bottom = m_Position.y;
	m_Bounds.width = m_Width;
	m_Bounds.height = m_Height;
	m_PtrBullet = new Bullet(6.0f, 6.0f, m_FieldBoundaries, m_PtrPlayer);
}

NPC::~NPC()
{
	delete m_PtrBullet;
}

void NPC::Draw() const
{
	if (m_IsDead == false and m_IsEnemy == true)
	{
		SetColor(Color4f{ 0.9f, 0.0f, 0.1f, 1.0f });
		FillRect(m_Bounds);
		m_PtrBullet->Draw();
	}
	else if (m_IsDead == false)
	{
		SetColor(Color4f{ 0.1f, 0.0f, 0.9f, 1.0f });
		FillRect(m_Bounds);
	}
}

void NPC::Update(float elapsedSec)
{
	m_PtrBullet->Update(elapsedSec);
}

bool NPC::DoHitTest(const Rectf& bullet)
{
	if (IsOverlapping(bullet, m_Bounds) and m_IsDead == false)
	{
		m_IsDead = true;
		return true;
	}
	else
	{
		return false;
	}
}

void NPC::Shoot(const Point2f& direction)
{
	Vector2f DirVector{ direction.x - m_Position.x, direction.y - m_Position.y };
	Vector2f Normalized{ DirVector.Normalized() };

	if (m_IsEnemy and !m_IsDead)
	{
		m_PtrBullet->Shoot(Point2f{ m_Position.x, m_Position.y }, Vector2f{ 600.0f * Normalized.x, 600.0f * Normalized.y });
	}
}

void NPC::SetPos(const Point2f& pos)
{
	m_Position = pos;
}

void NPC::SetDimensions(float width, float height)
{
	m_Width = width;
	m_Height = height;
}

bool NPC::IsDead() const
{
	return m_IsDead;
}

bool NPC::IsEnemy() const
{
	return m_IsEnemy;
}

Rectf NPC::GetBounds() const
{
	return m_Bounds;
}

Point2f NPC::GetPosition() const
{
	return m_Position;
}

void NPC::SetAlive()
{
	m_IsDead = false;
}
