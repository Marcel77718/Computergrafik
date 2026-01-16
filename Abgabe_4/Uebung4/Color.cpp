/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 4 	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////

#include "Color.h"
#include "cg_math.h"


Color::Color(double r, double g, double b) noexcept 
{
	this->r = r;
	this->g = g;
	this->b = b;
}

Color::Color(const CVec3d& color) noexcept
{
	this->r = crop(color[0]);
	this->g = crop(color[1]);
	this->b = crop(color[2]);
}

double& Color::operator [] (int i) noexcept {
	switch (i % 3) {
	case 0: return r;
	case 1: return g;
	case 2: return b;
	default: break;
	}
	return r; // unreachable dummy	
}

Color Color::operator + (const Color& x) const noexcept
{
	return Color(r + x.r, g + x.g, b + x.b);
}

Color Color::operator * (const CVec3d& x) const noexcept
{
	return Color(r * x[0], g * x[1], b * x[2]);
}

Color Color::operator * (const double& x) const noexcept
{
	return Color(r * x, g * x, b * x);
}

std::ostream& operator << (std::ostream& os, const Color& c)
{
	os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
	return os;
}