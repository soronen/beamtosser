#pragma once

#include "RTWeek.h"


class Camera
{
public:
	Camera(
		Point3 lookFrom,
		Point3 lookAt,
		Vec3   vUp,
		double vFOV, // vertical field-of-view in degrees
		double aspectRatio,
		double aperture,
		double focusDistance
	)
	{
		double theta = degreesToRadians(vFOV);
		double h = tan(theta / 2);
		double viewportHeight = 2.0 * h;
		double viewportWidth = aspectRatio * viewportHeight;

		Vec3 m_W = UnitVector(lookFrom - lookAt);
		Vec3 m_U = UnitVector(Cross(vUp, m_W));
		Vec3 m_V = Cross(m_W, m_U);

		m_Origin = lookFrom;
		m_Horizontal = focusDistance * viewportWidth * m_U;
		m_Vertical = focusDistance * viewportHeight * m_V;
		m_LowerLeftCorner = m_Origin - m_Horizontal / 2 - m_Vertical / 2 - focusDistance * m_W;

		m_LensRadius = aperture / 2;
	}


	Ray GetRay(double s, double t) const
	{
		Vec3 rd = m_LensRadius * RandomInUnitDisk();
		Vec3 offset = m_U * rd.x() + m_V * rd.y();

		return Ray(
			m_Origin + offset,
			m_LowerLeftCorner + s * m_Horizontal + t * m_Vertical - m_Origin - offset
		);
	}

private:
	Point3 m_Origin;
	Point3 m_LowerLeftCorner;
	Vec3 m_Horizontal;
	Vec3 m_Vertical;
	Vec3 m_U, m_V, m_W;
	double m_LensRadius;
};