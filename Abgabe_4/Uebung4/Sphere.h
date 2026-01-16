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

class Sphere
{
private:
	double r;		// radius
	CVec3d M;		// midpoint

public :
	Sphere(CVec3d _M, double _r) noexcept : M(_M), r(_r) {}

	void   setRadius(double _r)       noexcept { r = (_r>0)? _r: 0; }
	double getRadius(         ) const noexcept { return r; }

	void   setCenter(CVec3d _M)       noexcept { M = _M; }
	CVec3d getCenter(         ) const noexcept { return M; }

};
