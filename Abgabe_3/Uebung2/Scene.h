#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "glut.h" //for pixel drawing
#include "Color.h"


class Point {
public:

	Point(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
	}

	int x, y;
};

class Scene
{
public:
	//CVec4f projectZ(float fFocus, CVec4f pView);
	//CVec3f* constructQuader(CVec4f center, float width, float height, float depth);
	//void shiftQuader(CVec4f* quader, float x, float y, float z);
	//void drawProjektedZ(CVec3f Points[8]);
};

CVec3f* constructQuader(CVec4f center, float width, float height, float depth);
void shiftQuader(CVec4f* quader, float x, float y, float z);
CVec4f projectZ(float fFocus, CVec4f pView);
void drawProjektedZ(CVec3f Points[8]);
void BresenhamLine(Point p1, Point p2, Color c);
CVec3f* give_flat_quader();



