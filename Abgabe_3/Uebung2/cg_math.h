/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2+3	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Vector.h"
#include "Color.h"

// Conversion of indices of (x,y) coordinates to a linear array
#define TO_LINEAR(x, y) (((x)) + TEX_RES_X*((y)))

const double PI = 3.1415926535;

// crop x to interval [l,u]
double crop(double x, double l = 0.0, double u = 1.0);

// crop all componentens of x to interval [l,u]
CVec3d crop(const CVec3d& x, double l = 0.0, double u = 1.0);

// crop all componentens of x to interval [l,u]
Color crop(const Color& x, double l = 0.0, double u = 1.0);

// spherical to affine coordinates
// (x) PI <= XZ <= 2Pi (X)
// (y) 0  <= YZ <=  PI (Y)
CVec3d SphericalToAffine(const CVec3d& sphericalCoordinates);

