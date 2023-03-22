#pragma once

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable
{
public:
	Sphere() {}
	Sphere(Point3 center, double radius) : m_Center(center), m_Radius(radius) {};

	virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const override;

private:
	Point3 m_Center = Point3(0,0,0);
	double m_Radius = 0;
};

bool Sphere::Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const {
	Vec3 oc = r.Origin() - m_Center;
	double a = r.Direction().LengthSquared();
	double half_b = Dot(oc, r.Direction());
	double c = oc.LengthSquared() - m_Radius * m_Radius;

	double discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	double sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	double root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	record.t = root;
	record.p = r.At(record.t);
	record.normal = (record.p - m_Center) / m_Radius;

	Vec3 outwardNormal = (record.p - m_Center) / m_Radius;
	record.setFaceNormal(r, outwardNormal);

	return true;
}
