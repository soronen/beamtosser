#pragma once

#include "vec3.h"

#include <iostream>

void WriteColor(std::ostream& out, Color pixelColor, int samplesPerPixel)
{
	double r = pixelColor.x();
	double g = pixelColor.y();
	double b = pixelColor.z();

	// Divide the color by the number of samples and gamma-correct for gamma=2.0
	double scale = 1.0 / samplesPerPixel;
	r = std::sqrt(scale * r);
	g = std::sqrt(scale * g);
	b = std::sqrt(scale * b);

	// write the translated value of each color component
	out << static_cast<int>(256 * Clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << '\n';
}