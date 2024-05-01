#pragma once
#include <utils.h>
using namespace utils;

class NPC
{
public:
	NPC(const Point2f& pos, float width, float height, bool enemy);
	~NPC();
	void Draw() const;
	bool DoHitTest(const Rectf& bullet);
	void SetPos(const Point2f& pos);
	void SetDimensions(float width, float height);
	bool IsDead() const;

private:
	Rectf	GetBounds() const;

	Rectf	m_Bounds;
	Point2f m_Position;
	float	m_Width;
	float	m_Height;
	bool	m_IsDead;
	bool	m_IsEnemy;
};

