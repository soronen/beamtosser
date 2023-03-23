#include <iostream>

#include "RTWeek.h"

#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"


Color RayColor(const Ray& ray, const Hittable& world, int depth) {

	// return black if at the depth limit
	if (depth <= 0)
		return Color(0, 0, 0);


	HitRecord record;
	if (world.Hit(ray, 0.001, infinity, record))
	{
		Ray scattered;
		Color attenuation;

		if (record.materialPointer->Scatter(ray, record, attenuation, scattered))
			return attenuation * RayColor(scattered, world, depth - 1);

		Point3 target = record.p + RandomInHemisphere(record.normal);
		return 0.5 * RayColor(Ray(record.p, target - record.p), world, depth-1);
	}

	Vec3 unitDirection = UnitVector(ray.Direction());
	double t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main()
{

	// image
	const double aspectRatio = 16.0 / 9.0;
	const int imageWidth = 400;
	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	const int samplesPerPixel = 100;
	const int maxDepth = 50;

	// world
	HittableList world;

	auto ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto center = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
	auto left = std::make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
	auto right = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

	world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, ground));
	world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, center));
	world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, left));
	world.Add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, right));



	// camera
	Camera viewPoint;

	// render
	std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

	// top to bottom, left to right
	for (int i = imageHeight - 1; i >= 0; i--)
	{
		std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
		for (int j = 0; j < imageWidth; j++)
		{
			Color pixelColor(0, 0, 0);
			for (int s = 0; s < samplesPerPixel; s++)
			{
				double v = (i + RandomDouble()) / (imageHeight - 1);
				double u = (j + RandomDouble()) / (imageWidth - 1);
				Ray r = viewPoint.GetRay(u, v);
				pixelColor += RayColor(r, world, maxDepth);
			}
			WriteColor(std::cout, pixelColor, samplesPerPixel);
		}
	}

	return 0;
}