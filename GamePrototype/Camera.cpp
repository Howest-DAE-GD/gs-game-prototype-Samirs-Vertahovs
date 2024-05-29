#include "pch.h"
#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight) :
	m_ScreenWidth{ screenWidth },
	m_ScreenHeight{ screenHeight },
	m_TrackPos{}
{

}

Camera::~Camera()
{

}

void Camera::Aim(float levelW, float levelH, const Point2f& trackCenter)
{
	Point2f NewCameraPos{ trackCenter.x - m_ScreenWidth * 0.5f, trackCenter.y - m_ScreenHeight * 0.5f };

	if (NewCameraPos.x < 0.0f)
	{
		NewCameraPos.x = 0.0f;
	}
	if (NewCameraPos.y < 0.0f)
	{
		NewCameraPos.y = 0.0f;
	}

	if (NewCameraPos.x + m_ScreenWidth > levelW)
	{
		NewCameraPos.x = levelW - m_ScreenWidth;
	}
	if (NewCameraPos.y + m_ScreenHeight > levelH)
	{
		NewCameraPos.y = levelH - m_ScreenHeight;
	}

	m_TrackPos.x = NewCameraPos.x;
	m_TrackPos.y = NewCameraPos.y;

	glPushMatrix();
	{
		glTranslatef(-NewCameraPos.x, -NewCameraPos.y, 0.0f);
	}

}

void Camera::Reset()
{
	glPopMatrix();
}

Point2f Camera::GetTrackPos() const
{
	return m_TrackPos;
}