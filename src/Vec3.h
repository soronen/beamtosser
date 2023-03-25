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

	Vec3& operator+=(const Vec3& m_V)
	{
		m_Coordinates[0] += m_V.m_Coordinates[0];
		m_Coordinates[1] += m_V.m_Coordinates[1];
		m_Coordinates[2] += m_V.m_Coordinates[2];
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

	inline bool NearZero() const
	{
		// Return true if the vector is close to zero in all dimensions.
		const double s = 1e-8;
		return (fabs(m_Coordinates[0]) < s) && (fabs(m_Coordinates[1]) < s) && (fabs(m_Coordinates[2]) < s);

	}

private:
	double m_Coordinates[3];

};

using Point3 = Vec3;
using Color = Vec3;


// ïnline utility operators
inline std::ostream& operator<<(std::ostream& out, const Vec3& m_V)
{
	return out << m_V.x() << ' ' << m_V.y() << ' ' << m_V.z();
}

inline Vec3 operator+(const Vec3& m_U, const Vec3& m_V)
{
	return Vec3(m_U.x() + m_V.x(), m_U.y() + m_V.y(), m_U.z() + m_V.z());
}

inline Vec3 operator-(const Vec3& m_U, const Vec3& m_V)
{
	return Vec3(m_U.x() - m_V.x(), m_U.y() - m_V.y(), m_U.z() - m_V.z());
}

inline Vec3 operator*(const Vec3& m_U, const Vec3& m_V)
{
	return Vec3(m_U.x() * m_V.x(), m_U.y() * m_V.y(), m_U.z() * m_V.z());
}

inline Vec3 operator*(double t, const Vec3& m_V)
{
	return Vec3(t * m_V.x(), t * m_V.y(), t * m_V.z());
}

inline Vec3 operator*(const Vec3& m_V, double t)
{
	return t * m_V;
}

inline Vec3 operator/(Vec3 m_V, double t)
{
	return (1 / t) * m_V;
}

inline double Dot(const Vec3& m_U, const Vec3& m_V)
{
	return m_U.x() * m_V.x()
		+ m_U.y() * m_V.y()
		+ m_U.z() * m_V.z();
}

inline Vec3 Cross(const Vec3& m_U, const Vec3& m_V)
{
	return Vec3(m_U.y() * m_V.z() - m_U.z() * m_V.y(),
		m_U.z() * m_V.x() - m_U.x() * m_V.z(),
		m_U.x() * m_V.y() - m_U.y() * m_V.x());
}

inline Vec3 UnitVector(Vec3 m_V)
{
	return m_V / m_V.Length();
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

inline Vec3 Reflect(const Vec3& m_V, const Vec3& n)
{
	return m_V - 2 * Dot(m_V, n) * n;
}

inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etai_over_etat)
{
	double cosTheta = fmin(Dot(-uv, n), 1.0);
	Vec3 rOutPerpetual = etai_over_etat * (uv + cosTheta * n);
	Vec3 rOutParallel = -sqrt(fabs(1.0 - rOutPerpetual.LengthSquared())) * n;
	return rOutPerpetual + rOutParallel;
}

inline Vec3 RandomInUnitDisk()
{
	while (true)
	{
		Vec3 p = Vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
		if (p.LengthSquared() >= 1) 
			continue;
		return p;
	}
}