#include "pch.h"
#include "NPC.h"
#include "Bullet.h"
#include "Player.h"
#include <iostream>

NPC::NPC(const Point2f& pos, float width, float height, bool enemy, const Rectf& fieldboundaries, Player* player) :
	m_Position{ pos },
	m_Width{ width },
	m_FieldBoundaries{ fieldboundaries },
	m_Height{ height },
	m_IsDead{ false },
	m_IsEnemy{ enemy },
	m_Bounds { },
	m_MovingPeriod{ },
	m_Velocity{ },
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
	if (!m_IsDead and m_IsEnemy)
	{
		SetColor(Color4f{ 230 / 255.0f, 60 / 255.0f, 60 / 255.0f, 1.0f });
		FillRect(Rectf{ m_Bounds.left - 3.0f, m_Bounds.bottom - 3.0f, m_Bounds.width + 6.0f, m_Bounds.height + 6.0f });

		SetColor(Color4f{ 250 / 255.0f, 90 / 255.0f, 90 / 255.0f, 1.0f });
		FillRect(m_Bounds);
		m_PtrBullet->Draw();
	}
	else if (!m_IsDead)
	{
		SetColor(Color4f{ 99 / 255.0f, 87 / 255.0f, 220 / 255.0f, 1.0f });
		FillRect(Rectf{ m_Bounds.left - 3.0f, m_Bounds.bottom - 3.0f, m_Bounds.width + 6.0f, m_Bounds.height + 6.0f });

		SetColor(Color4f{ 129 / 255.0f, 117 / 255.0f, 250 / 255.0f, 1.0f });
		FillRect(m_Bounds);
	}

}

void NPC::Update(float elapsedSec)
{
	m_PtrBullet->Update(elapsedSec);

	if (m_MovingPeriod > 0.0f)
	{
		m_MovingPeriod -= elapsedSec;
	}

	if (m_MovingPeriod < 0.0f)
	{
		m_MovingPeriod = 0.0f;
	}

	if (m_MovingPeriod == 0.0f)
	{
		m_Velocity.x = rand() % 401 - 200.0f;
		m_Velocity.y = rand() % 401 - 200.0f;
		m_MovingPeriod = 2.25f;
	}

	m_Position.x += m_Velocity.x * elapsedSec;
	m_Position.y += m_Velocity.y * elapsedSec;

	m_Bounds.left = m_Position.x;
	m_Bounds.bottom = m_Position.y;

	if (m_Position.x < 0.0f)
	{
		m_Position.x = 0.0f;
	}

	if (m_Position.x > 2970.0f)
	{
		m_Position.x = 2970.0f;
	}

	if (m_Position.y < 0.0f)
	{
		m_Position.y = 0.0f;
	}

	if (m_Position.y > 1970.0f)
	{
		m_Position.y = 1970.0f;
	}
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

	if (m_IsEnemy and !m_IsDead and m_PtrPlayer->IsAlive())
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

void NPC::SetHostility(bool isEnemy)
{
	m_IsEnemy = isEnemy;
}
