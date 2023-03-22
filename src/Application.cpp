#include <iostream>

#include "RTWeek.h"

#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"


Color RayColor(const Ray& ray, const Hittable& world, int depth) {

	// return black if at the depth limit
	if (depth <= 0)
		return Color(0, 0, 0);


	HitRecord record;
	if (world.Hit(ray, 0, infinity, record))
	{
		Vec3 target = record.p + record.normal + RandomInUnitSphere();
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
	world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

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