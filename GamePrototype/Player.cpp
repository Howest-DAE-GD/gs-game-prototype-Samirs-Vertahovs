#include "pch.h"
#include "Player.h"
#include <iostream>

Player::Player(const Point2f& pos, float width, float height, const Rectf& fieldboundaries) :
	m_Position			{ pos },
	m_Width				{ width },
	m_Height			{ height },
	m_FieldBoundaries	{ fieldboundaries },
	m_Velocity			{ 0.0f, 0.0f },
	m_BulletSpeed		{ },
	m_Alive				{ true },
	m_Bounds			{ },
	m_Counter			{ }
{
	m_Bounds.left		= m_Position.x;
	m_Bounds.bottom		= m_Position.y;
	m_Bounds.width		= m_Width;
	m_Bounds.height		= m_Height;
	m_PtrBullet			= new Bullet(6.0f, 6.0f, m_FieldBoundaries);
}
Player::~Player()
{
	delete m_PtrBullet;
}

void Player::Update(float elapsedSec, std::vector<NPC*>& npcs, const Uint8* pStates)
{
	bool check = false;

	m_PtrBullet->Update(elapsedSec, npcs, m_Counter);

	if (m_Alive)
	{
		if (pStates[SDL_SCANCODE_W])
		{
			m_Velocity.y = 350.0f;
			m_Position.y += m_Velocity.y * elapsedSec;
			m_Bounds.bottom = m_Position.y;
			check = true;
		}
		if (pStates[SDL_SCANCODE_S])
		{
			m_Velocity.y = -350.0f;
			m_Position.y += m_Velocity.y * elapsedSec;
			m_Bounds.bottom = m_Position.y;
			check = true;
		}
		if (pStates[SDL_SCANCODE_D])
		{
			m_Velocity.x = 350.0f;
			m_Position.x += m_Velocity.x * elapsedSec;
			m_Bounds.left = m_Position.x;
			check = true;
		}
		if (pStates[SDL_SCANCODE_A])
		{
			m_Velocity.x = -350.0f;
			m_Position.x += m_Velocity.x * elapsedSec;
			m_Bounds.left = m_Position.x;
			check = true;
		}
	}

	if (!check)
	{
		m_Velocity.x = 0.0f;
		m_Velocity.y = 0.0f;
	}

	m_Bounds.left = m_Position.x;
	m_Bounds.bottom = m_Position.y;
	m_Bounds.width = m_Width;
	m_Bounds.height = m_Height;

	std::cout << m_Counter << std::endl;

}

void Player::Draw() const
{
	if (m_Alive)
	{
		SetColor(Color4f{ 1.0f, 0.4117f, 0.7058f, 1.0f });
	}
	else
	{
		SetColor(Color4f{ 0.35f, 0.35f, 0.35f, 1.0f });
	}

	FillRect(m_Bounds);
	m_PtrBullet->Draw();
}

void Player::Shoot(const Point2f& direction) //add dependancy on the click direction ---- DONE
{
	Vector2f DirVector{ (direction.x + m_Position.x - 850.0f * 0.5f) - m_Position.x, (direction.y + m_Position.y - 500.0f * 0.5f) - m_Position.y};
	Vector2f Normalized{ DirVector.Normalized() };

	if (m_Alive)
	{
		m_PtrBullet->Shoot(Point2f{ m_Position.x + 17.5f, m_Position.y + 17.5f }, Vector2f{ 800.0f * Normalized.x, 800.0f * Normalized.y });
	}
}

bool Player::DoHitTest(const Rectf& bullet)
{
	if (IsOverlapping(bullet, m_Bounds) and m_Alive)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Point2f Player::GetPosition() const
{
	return m_Position;
}

Vector2f Player::GetVelocity() const
{
	return m_Velocity;
}

Rectf Player::GetBounds() const
{
	return m_Bounds;
}

void Player::Die()
{
	m_Alive = false;
}

void Player::SetAlive()
{
	m_Alive = true;
}

void Player::SetPos(const Point2f& pos)
{
	m_Position = pos;
}

