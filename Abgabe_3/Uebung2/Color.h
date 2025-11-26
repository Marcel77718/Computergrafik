/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2+3	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////
//
#pragma once

#include "Vector.h"
#include <iostream>

class Color {
public:
	double r, g, b;

public:

	Color(double r = 1.0, double g = 1.0, double b = 1.0);
	Color(const CVec3d& color);

	double& operator [] (int i);
	Color   operator * (const CVec3d& x) const;		// Hadamar product
};

std::ostream& operator << (std::ostream& os, const Color& c);

const Color Red    (1.0, 0.0, 0.0);
const Color Green  (0.0, 1.0, 0.0);
const Color Blue   (0.0, 0.0, 1.0);
const Color Yellow (1.0, 1.0, 0.0);
const Color Cyan   (0.0, 1.0, 1.0);
const Color Magenta(1.0, 0.0, 1.0);
const Color White  (1.0, 1.0, 1.0);
const Color MoonGrey(0.5, 0.5, 0.5);
const Color EarthBlue(0.15, 0.2, 0.6);
const Color SunYellow(1.0, 0.95, 0.3);
