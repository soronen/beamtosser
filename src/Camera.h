#pragma once

#include "RTWeek.h"

class Camera
{
public:
	Camera(Point3 lookFrom, Point3 lookAt, Vec3 vUp, double vFOV, double aspectRatio) 
	{
		double theta = degreesToRadians(vFOV); 

		double h = tan(theta / 2);

		double viewportHeight = 2.0 * h;
		double viewportWidth = aspectRatio * viewportHeight;

		// opposite direction that camera faces;
		Vec3 w = UnitVector(lookFrom - lookAt);

		Vec3 u = UnitVector(Cross(vUp, w));
		Vec3 v = Cross(w, u);

		m_Origin = lookFrom;
		m_Horizontal = viewportWidth * u;
		m_Vertical = viewportHeight * v;
		m_LowerLeftCorner = m_Origin -m_Horizontal / 2 - m_Vertical / 2 - w;
	}

	// takes in horizontal and vertical pixel coordinates,
	// returns ray that starts from camera and points to the pixel
	Ray GetRay(double s, double t) const 
	{
		return Ray(m_Origin, m_LowerLeftCorner + s * m_Horizontal + t * m_Vertical - m_Origin);
	}

private:
	Point3 m_Origin;
	Point3 m_LowerLeftCorner;
	Vec3 m_Horizontal;
	Vec3 m_Vertical;
};