/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2  	//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////


#include "Planet.h"
#include "glut.h"

//
// Implement HERE your Bresenham algorithm for circles 
//
void Planet::draw() const
{
	glBegin(GL_POINTS);
	glColor3d(color.r, color.g, color.b);
	// ...

	// implement BresenhamCircle here and use
	// glVertex2i (x, y);
	// to draw a pixel at (x,y)

	// ...
	glEnd();
}

