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

	//std::cout << m_Counter << std::endl;

}

void Player::Draw() const
{

	if (m_Alive)
	{
		SetColor(Color4f{ 0.8f, 0.2117f, 0.5058f, 1.0f });
	}
	else
	{
		SetColor(Color4f{ 0.1f, 0.1f, 0.1f, 1.0f });
	}

	FillRect(Rectf{ m_Bounds.left - 3.0f, m_Bounds.bottom - 3.0f, m_Bounds.width + 6.0f, m_Bounds.height + 6.0f  });

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
	Point2f playerPos = m_Position;
	Point2f start{ playerPos.x + 17.5f, playerPos.y + 17.5f };
	Point2f end;

	if (playerPos.x > 425.0f && playerPos.x < 2575.0f)
	{
		if (playerPos.y > 250.0f && playerPos.y < 1750.0f)
		{
			end = Point2f{ direction.x + playerPos.x - 850.0f * 0.5f, direction.y + playerPos.y - 500.0f * 0.5f };
		}
		else if (playerPos.y < 250.0f)
		{
			end = Point2f{ direction.x + playerPos.x - 850.0f * 0.5f, direction.y };
		}
		else if (playerPos.y > 1750.0f)
		{
			end = Point2f{ direction.x + playerPos.x - 850.0f * 0.5f, direction.y + (2000.0f - 500.0f) };
		}
	}
	else if (playerPos.x < 425.0f)
	{
		if (playerPos.y > 250.0f && playerPos.y < 1750.0f)
		{
			end = Point2f{ direction.x, direction.y + playerPos.y - 500.0f * 0.5f };
		}
		else if (playerPos.y < 250.0f)
		{
			end = Point2f{ direction.x, direction.y };
		}
		else if (playerPos.y > 1750.0f)
		{
			end = Point2f{ direction.x, direction.y + (2000.0f - 500.0f) };
		}
	}
	else if (playerPos.x > 2575.0f)
	{
		if (playerPos.y > 250.0f && playerPos.y < 1750.0f)
		{
			end = Point2f{ direction.x + (3000.0f - 850.0f), direction.y + playerPos.y - 500.0f * 0.5f };
		}
		else if (playerPos.y < 250.0f)
		{
			end = Point2f{ direction.x + (3000.0f - 850.0f), direction.y };
		}
		else if (playerPos.y > 1750.0f)
		{
			end = Point2f{ direction.x + (3000.0f - 850.0f), direction.y + (2000.0f - 500.0f) };
		}
	}


	Vector2f DirVector{ start, end };

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

bool Player::IsAlive() const
{
	return m_Alive;
}

void Player::SetCounter(int counter) 
{
	m_Counter = counter;
}