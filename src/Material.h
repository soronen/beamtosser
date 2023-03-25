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
		scattered = Ray(rec.p, reflected + m_Fuzz * RandomInUnitSphere());
		attenuation = m_Albedo;
		return (Dot(scattered.Direction(), rec.normal) > 0);
	}

private:
	Color m_Albedo;
	double m_Fuzz;

};

class Dielectric : public Material
{
public:
	Dielectric(double indexOfRefraction) : m_IndexOfRefraction(indexOfRefraction) {}
	virtual bool Scatter(
		const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered
	) const override {
		attenuation = Color(1.0, 1.0, 1.0);
		double refractionRatio = rec.frontFace ? (1.0 / m_IndexOfRefraction) : m_IndexOfRefraction;

		Vec3 unitDirection = UnitVector(r_in.Direction());

		double cosTheta = fmin(Dot(-unitDirection, rec.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		Vec3 direction;

		if (cannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble())
			direction = Reflect(unitDirection, rec.normal);
		else
			direction = Refract(unitDirection, rec.normal, refractionRatio);

		scattered = Ray(rec.p, direction);
		return true;
	}

private:
	double m_IndexOfRefraction;

private:
	static double Reflectance(double cosine, double reflectionIndex)
	{
		// Schlick approgimation for reflectance
		double r0 = (1 - reflectionIndex) / (1 + reflectionIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);

	}
};