#include "pch.h"
#include "NPC.h"

NPC::NPC(const Point2f& pos, float width, float height, bool enemy) :
	m_Position{ pos },
	m_Width{ width },
	m_Height{ height },
	m_IsDead{ false },
	m_IsEnemy{ enemy },
	m_Bounds{}
{
	m_Bounds.left = m_Position.x;
	m_Bounds.bottom = m_Position.y;
	m_Bounds.width = m_Width;
	m_Bounds.height = m_Height;
}

NPC::~NPC()
{

}

void NPC::Draw() const
{
	if (m_IsDead == false and m_IsEnemy == true)
	{
		SetColor(Color4f{ 1.0f, 0.0f, 0.0f, 1.0f });
		FillRect(m_Bounds);
	}
	else if (m_IsDead == false)
	{
		SetColor(Color4f{ 0.0f, 0.0f, 1.0f, 1.0f });
		FillRect(m_Bounds);
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

Rectf NPC::GetBounds() const
{
	return m_Bounds;
}
