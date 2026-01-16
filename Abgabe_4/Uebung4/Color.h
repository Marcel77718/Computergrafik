/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 4 	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Vector.h"
#include <iostream>

class Color {
public:
	double r, g, b;

public:

	Color(double r = 1.0, double g = 1.0, double b = 1.0) noexcept;
	Color(const CVec3d& color) noexcept;

	double& operator [] (int i) noexcept;

	Color   operator + (const Color&  x) const noexcept;		// component-wise sum
	Color   operator * (const CVec3d& x) const noexcept;		// Hadamar        product
	Color   operator * (const double& x) const noexcept;		//                product

	void   red  (double _r) noexcept { r = _r; }
	void   green(double _g) noexcept { r = _g; }
	void   blue (double _b) noexcept { r = _b; }
	double red  () const    noexcept { return r; }
	double green() const    noexcept { return g; }
	double blue () const    noexcept { return b; }
};

std::ostream& operator << (std::ostream& os, const Color& c);


const Color Red(1.0, 0.0, 0.0);
const Color Green(0.0, 1.0, 0.0);
const Color Blue(0.0, 0.0, 1.0);
const Color Yellow(1.0, 1.0, 0.0);
const Color Cyan(0.0, 1.0, 1.0);
const Color Magenta(1.0, 0.0, 1.0);
