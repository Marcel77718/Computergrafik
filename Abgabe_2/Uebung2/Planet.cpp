/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2  	//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////


#include "Planet.h"
#include "glut.h"
#include "Matrix.h"
#include <math.h>

//
// Implement HERE your Bresenham algorithm for circles 
//
void Planet::draw() const
{
	glBegin(GL_POINTS);
	glColor3d(color.r, color.g, color.b);
	// ...
	int r = this->radius;
	int p_x = static_cast<int>(this->position[0]);
	int p_y = static_cast<int>(this->position[1]);

	int x, y, d, dse, de;
	x = 0; y = r;
	d = 5 - 4 * r;

	auto plot = [&](int x, int y) {
		glVertex2i(p_x + x, p_y + y);
		glVertex2i(p_x - x, p_y + y);
		glVertex2i(p_x + x, p_y - y);
		glVertex2i(p_x - x, p_y - y);
		glVertex2i(p_x + y, p_y + x);
		glVertex2i(p_x - y, p_y + x);
		glVertex2i(p_x + y, p_y - x);
		glVertex2i(p_x - y, p_y - x);
		};

	plot(x, y);

	while (y > x) {
		if (d >= 0) { dse = 4 * (2 * (x - y) + 5); d += dse; x++; y--; }
		else { de = 4 * (2 * x + 3); d += de; x++; }
		plot(x, y);
	}

	// implement BresenhamCircle here and use
	// glVertex2i (x, y);
	// to draw a pixel at (x,y)

	// ...
	glEnd();
}

void Planet::rotate(float theta, CVec2f center)
{
	this->position = CVec2f(
		center[0] + (this->position[0] - center[0]) * cos(theta) - (this->position[1] - center[1]) * sin(theta),
		center[1] + (this->position[0] - center[0]) * sin(theta) + (this->position[1] - center[1]) * cos(theta)
	);
}

