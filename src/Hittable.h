#pragma once

#include "Ray.h"

struct HitRecord
{
	Point3 p = Point3(0,0,0);
	Vec3 normal = Vec3(0,0,0);
	double t = 0;
	bool frontFace = true;

	 // normals always point outward from the surface
	inline void setFaceNormal(const Ray& r, const Vec3& outwardNormal)
	{
		frontFace = Dot(r.Direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const = 0;
};