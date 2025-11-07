/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2 	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Vector.h"
#include "Color.h"

class Planet {
private:
	Color  color    = Red;
	int    radius = 10;
	CVec2f position = CVec2f(0.0);

public:
	Planet() { }

	// getter
	Color  getColor   () const { return color; }
	int    getRadius  () const { return radius; }
	CVec2f getPosition() const { return position; }

	// setter
	void setColor   (const Color&  c) { color    = c; }
	void setRadius  (int           r) { radius   = std::max(1, r); }
	void setPosition(const CVec2f& p) { position = p; }

	// draw
	void draw() const; // Implement in this method your Bresenham algorithm for circles 

	//rotation
	void rotate(float theta, CVec2f center);

};