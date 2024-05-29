#pragma once
class Camera
{
public:
	Camera(float screenWidth, float screenHeight);
	~Camera();

	void Aim(float levelW, float levelH, const Point2f& trackCenter);
	void Reset();

	Point2f GetTrackPos() const;

private:
	float m_ScreenWidth;
	float m_ScreenHeight;
	Point2f m_TrackPos;

};

