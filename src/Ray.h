#pragma once

#include "Vec3.h"

class Ray
{
public:
	Ray() {}
	Ray(const Point3& m_Origin, const Vec3& m_Direction)
		: m_Origin(m_Origin), m_Direction(m_Direction)
	{}

	Point3 Origin() const { return m_Origin; }
	Vec3 Direction() const { return m_Direction; }

	Point3 At(double t) const { return m_Origin + t * m_Direction; }

private:
	Point3 m_Origin;
	Vec3 m_Direction;
};