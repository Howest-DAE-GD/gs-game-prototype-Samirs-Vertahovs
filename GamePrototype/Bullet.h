#pragma once
#include "NPC.h"
#include <utils.h>

using namespace utils;

class Player;

class Bullet
{
public:
	Bullet(float width, float height, const Rectf& fieldboundaries, Player* player);
	Bullet(float width, float height, const Rectf& fieldboundaries);
	~Bullet();

	void Draw() const;
	void Update(float elapsedSec, std::vector<NPC*>& npcs, int& killedEnemies, int& killedCivs);
	void Update(float elapsedSec);
	void Shoot(const Point2f& pos, const Vector2f& velocity);

private:
	void CheckBoundaries();
	void CheckHit(std::vector<NPC*>& npcs, int& killedEnemies, int& killedCivs);

	Point2f m_Position;
	float m_Width;
	float m_Height;
	Vector2f m_Velocity;
	Rectf m_FieldBoundaries;
	bool m_IsActivated;
	float m_Time;
	Player* m_PtrPlayer;
};

