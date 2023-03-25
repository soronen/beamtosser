#include <iostream>

#include "RTWeek.h"

#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"

#include <omp.h>

HittableList RandomScene() {
	HittableList world;

	auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = RandomDouble();
			Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

			if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					Color albedo = Color::Random() * Color::Random();
					sphere_material = std::make_shared<Lambertian>(albedo);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					Color albedo = Color::Random(0.5, 1);
					double fuzz = RandomDouble(0, 0.5);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(1.5);
					world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5);
	world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	return world;
}

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
		return 0.5 * RayColor(Ray(record.p, target - record.p), world, depth - 1);
	}

	Vec3 unitDirection = UnitVector(ray.Direction());
	double t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main()
{

	// image
	const double aspectRatio = 3.0 / 2.0;
	const int imageWidth = 3840;
	const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
	const int samplesPerPixel = 500;
	const int maxDepth = 50;

	// world
	auto world = RandomScene();


	// camera
	Point3 lookFrom(13, 2, 3);
	Point3 lookAt(0, 0, 0);
	Vec3 vUp(0, 1, 0);
	double distToFocus = 10.0;
	double aperture = 0.1;

	Camera viewPoint(lookFrom, lookAt, vUp, 20, aspectRatio, aperture, distToFocus);

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
				double m_V = (i + RandomDouble()) / (imageHeight - 1);
				double m_U = (j + RandomDouble()) / (imageWidth - 1);
				Ray r = viewPoint.GetRay(m_U, m_V);
				pixelColor += RayColor(r, world, maxDepth);
			}
			WriteColor(std::cout, pixelColor, samplesPerPixel);
		}
	}

	return 0;
}