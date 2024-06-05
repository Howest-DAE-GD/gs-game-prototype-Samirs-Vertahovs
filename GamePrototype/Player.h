#pragma once
#include "Bullet.h"
#include "NPC.h"
#include <utils.h>
#include <vector>
using namespace utils;

class Player
{
public:
	Player(const Point2f& pos, float width, float height, const Rectf& fieldboundaries);
	~Player();

	void Update(float elapsedSec, std::vector<NPC*>& enemies, const Uint8* pStates, int& killedEnemies, int& killedCivs);
	void Draw() const;
	void Shoot(const Point2f& direction);
	bool DoHitTest(const Rectf& bullet);
	void Die();
	void SetAlive();
	void SetPos(const Point2f& pos);
	void SetCounter(int counter);

	bool IsAlive() const;
	Point2f GetPosition() const;
	Vector2f GetVelocity() const;

private:
	Rectf GetBounds() const;

	Bullet* m_PtrBullet;
	Point2f m_Position;
	Vector2f m_Velocity;
	float m_Width;
	float m_Height;
	Rectf m_Bounds;
	Rectf m_FieldBoundaries;
	float m_BulletSpeed;
	bool m_Alive;
	int	 m_Counter;
};

