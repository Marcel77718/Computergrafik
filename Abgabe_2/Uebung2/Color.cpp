/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2+3	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////


#include "Color.h"
#include "cg_math.h"


Color::Color(double r, double g, double b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

Color::Color(const CVec3d& color)
{
	this->r = crop(color[0]);
	this->g = crop(color[1]);
	this->b = crop(color[2]);
}

double& Color::operator [] (int i) {
	switch (i % 3) {
	case 0: return r;
	case 1: return g;
	case 2: return b;
	}
	return r; // unreachable dummy	
}

Color Color::operator * (const CVec3d& x) const
{
	return Color(r * x[0], g * x[1], b * x[2]);
}

std::ostream& operator << (std::ostream& os, const Color& c)
{
	os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
	return os;
}