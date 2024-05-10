#pragma once
#include <utils.h>

using namespace utils;

class Bullet;

class Player;

class NPC
{
public:
	NPC(const Point2f& pos, float width, float height, bool enemy, const Rectf& fieldboundaries, Player* player);
	~NPC();
	void Draw() const;
	void Update(float elapsedSec);
	bool DoHitTest(const Rectf& bullet);
	void SetPos(const Point2f& pos);
	void SetDimensions(float width, float height);
	void SetAlive();

	void Shoot(const Point2f& direction);
	bool IsDead() const;
	bool IsEnemy() const;

	Point2f GetPosition() const;

private:
	Rectf	GetBounds() const;

	Rectf	m_Bounds;
	Rectf   m_FieldBoundaries;
	Point2f m_Position;
	float	m_Width;
	float	m_Height;
	bool	m_IsDead;
	bool	m_IsEnemy;
	Bullet* m_PtrBullet;
	Player* m_PtrPlayer;


};

