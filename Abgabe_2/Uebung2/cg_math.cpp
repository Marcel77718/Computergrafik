/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2+3	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////

#include "cg_math.h"
#include <math.h>

// crop x to interval [l,u]
double crop(double x, double l, double u)
{
	return std::min(u, std::max(l, x));
}

// crop all componentens of x to interval [l,u]
CVec3d crop(const CVec3d& x, double l, double u)
{
	return CVec3d(crop(x[0], l, u), crop(x[1], l, u), crop(x[2], l, u));
}

// crop all componentens of x to interval [l,u]
Color crop(const Color& x, double l, double u)
{
	return Color(crop(x.r, l, u), crop(x.g, l, u), crop(x.b, l, u));
}

// spherical to affine coordinates
// (x) PI <= XZ <= 2Pi (X)
// (y)  0 <= YZ <=  PI (Y)
CVec3d SphericalToAffine(const CVec3d& sphericalCoordinates)
{
	CVec3d affineCoordinates;
	affineCoordinates[0] = sphericalCoordinates[0] * sin(sphericalCoordinates[2]) * cos(sphericalCoordinates[1]);
	affineCoordinates[2] = sphericalCoordinates[0] * sin(sphericalCoordinates[2]) * sin(sphericalCoordinates[1]);
	affineCoordinates[1] = sphericalCoordinates[0] * cos(sphericalCoordinates[2]);
	//std::cout << "L= " << affineCoordinates << std::endl;
	return affineCoordinates;
}
