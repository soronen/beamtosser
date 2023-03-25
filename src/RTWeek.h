#pragma once

#include <cmath>
#include <limits>
#include <memory>


// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// utility functions
inline double degreesToRadians(double degrees)
{
	return degrees * pi / 180;
}


inline double RandomDouble()
{
	// random number between 0 and 1
	return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max)
{
	return min + (max - min) * RandomDouble();
}

inline double Clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

#include "Ray.h"
#include "Vec3.h"