#pragma once

#include "RTWeek.h"

#include "Hittable.h"

class Material
{
public:
	virtual bool Scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const Color& a) : m_Albedo(a) {}

	virtual bool Scatter(const Ray& ray_in, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		Vec3 scatterDirection = record.normal + RandomUnitVector();

		if (scatterDirection.NearZero())
			scatterDirection = record.normal;

		scattered = Ray(record.p, scatterDirection);
		attenuation = m_Albedo;
		return true;
	}

private:
	Color m_Albedo;
};


class Metal : public Material
{
public:
	Metal(const Color& a, double f) : m_Albedo(a), m_Fuzz(f < 1 ? f : 1) {}

	virtual bool Scatter(const Ray& ray_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = Reflect(UnitVector(ray_in.Direction()), rec.normal);
		scattered = Ray(rec.p, reflected + m_Fuzz*RandomInUnitSphere());
		attenuation = m_Albedo;
		return (Dot(scattered.Direction(), rec.normal) > 0);
	}

private:
	Color m_Albedo;
	double m_Fuzz;

};