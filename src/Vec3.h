#pragma once

#include "RTWeek.h"

#include <cmath>
#include <iostream>

class Vec3
{
public:
	Vec3() :m_Coordinates{ 0,0,0 } {}
	Vec3(double x, double y, double z) : m_Coordinates{ x, y, z } {}

	double x() const { return m_Coordinates[0]; }
	double y() const { return m_Coordinates[1]; }
	double z() const { return m_Coordinates[2]; }

	double Length() const
	{
		return std::sqrt(LengthSquared());
	}

	double LengthSquared() const
	{
		return m_Coordinates[0] * m_Coordinates[0] + m_Coordinates[1] * m_Coordinates[1] + m_Coordinates[2] * m_Coordinates[2];
	}

	Vec3 operator-() const { return Vec3(-m_Coordinates[0], -m_Coordinates[1], -m_Coordinates[2]); }
	double operator[](int i) const { return m_Coordinates[i]; }
	double& operator[](int i) { return m_Coordinates[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		m_Coordinates[0] += v.m_Coordinates[0];
		m_Coordinates[1] += v.m_Coordinates[1];
		m_Coordinates[2] += v.m_Coordinates[2];
		return *this;
	}

	Vec3& operator *= (const double t)
	{
		m_Coordinates[0] += t;
		m_Coordinates[1] += t;
		m_Coordinates[2] += t;
		return *this;
	}

	Vec3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	inline static Vec3 Random()
	{
		return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	inline static Vec3 Random(double min, double max)
	{
		return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
	}

private:
	double m_Coordinates[3];

};

using Point3 = Vec3;
using Color = Vec3;


// ïnline utility operators
inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vec3 operator+(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(Vec3 v, double t)
{
	return (1 / t) * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
	return u.x() * v.x()
		+ u.y() * v.y()
		+ u.z() * v.z();
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x());
}

inline Vec3 UnitVector(Vec3 v)
{
	return v / v.Length();
}

// this is used for reflection rays
inline Vec3 RandomInUnitSphere()
{
	while (true)
	{
		auto p = Vec3::Random(-1, 1);
		if (p.LengthSquared() >= 1)
			continue;
		return p;
	}
}

// same 
inline Vec3 RandomUnitVector()
{
	return UnitVector(RandomInUnitSphere());
}

// same 
inline Vec3 RandomInHemisphere(const Vec3& normal)
{
	Vec3 inUnitSphere = RandomInUnitSphere();
	if (Dot(inUnitSphere, normal) > 0.0) // in the same hemisphere as normal
		return inUnitSphere;
	else
		return -inUnitSphere;
}