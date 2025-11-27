#include "Scene.h"

// From the assignment description:
//- the view-origin ViewOrigin is the origin of the view-coordinate system,
//- the view-direction ViewDir is anti-parallel to the die zzvv-axis (and initially also the zzww-axis),
//- the view-up-vector ViewUp (yyvv-axis of the image plane) is initially parallel to the yyww-axis,
//- the eye-point EyePoint is on the positive zzvv-axis, i.e. (0,0,fFocus) in view-coordinates,
//- the image-plane is the xxvv yvv-plane,
//- the focal-distance fFocus is the distance of the eye-point to the view-origin, and
//- the focal-distance fFocus is a variable parameter of the function projectZ.

CVec4f projectZ(float fFocus, CVec4f pView)
{
    //check if pView is in the plane of the eye point
	if (fFocus - pView[2] <= 1e-6f) //using a small epsilon to avoid numerical issues
    {
		// Point is at infinity. We return a point far away and in the direction of pView.
		// This may be bad for huge objects but should be fine for our use case.
		// We could also set w to 0. But we check here to not divide by zero later.
		float a[4] = { pView[0] * 1e6f, pView[1] * 1e6f, 0.0f, 1.0f };
        return CVec4f(a);
	}

    //the real projection

	float x_proj = (fFocus / (fFocus - pView[2])) * pView[0];
	float y_proj = (fFocus / (fFocus - pView[2])) * pView[1];
	float a[4] = { x_proj, y_proj, 0.0f, 1.0f };
    return CVec4f(a);
}

CVec3f* constructQuader(CVec4f center, float width, float height, float depth)
{
	if (center[3] != 1.0f) // check if homogenous
	{
		center[0] /= center[3];
		center[1] /= center[3];
		center[2] /= center[3];
		center[3] = 1.0f;
	}
	CVec3f vertices[8]; //indices are connected so that the same modulo(4)s are connected to each other and ajacent indices as well.
	vertices[0] = CVec3f(center[0] - width / 2, center[1] - height / 2, center[2] - depth / 2);
	vertices[1] = CVec3f(center[0] + width / 2, center[1] - height / 2, center[2] - depth / 2);
	vertices[2] = CVec3f(center[0] + width / 2, center[1] + height / 2, center[2] - depth / 2);
	vertices[3] = CVec3f(center[0] - width / 2, center[1] + height / 2, center[2] - depth / 2);
	vertices[4] = CVec3f(center[0] - width / 2, center[1] - height / 2, center[2] + depth / 2);
	vertices[5] = CVec3f(center[0] + width / 2, center[1] - height / 2, center[2] + depth / 2);
	vertices[6] = CVec3f(center[0] + width / 2, center[1] + height / 2, center[2] + depth / 2);
	vertices[7] = CVec3f(center[0] - width / 2, center[1] + height / 2, center[2] + depth / 2);
	return vertices;
}

void shiftQuader(CVec4f* quader, float x, float y, float z)
{
	for (int i = 0; i < 8; i++)
	{
		if (quader[i][3] != 1.0f) // check if homogenous
		{
			quader[i][0] /= quader[i][3];
			quader[i][1] /= quader[i][3];
			quader[i][2] /= quader[i][3];
			quader[i][3] = 1.0f;
		}
		quader[i][0] += x;
		quader[i][1] += y;
		quader[i][2] += z;
	}
}

void drawProjektedZ(CVec3f Points[8])
{
	glBegin(GL_POINTS);
	glColor3d(0.7, 0.7, 0.0);

	// trying to render the first line first before implementing all the others to see if usage of gl and Bresenham is correct.
	int p0_x = static_cast<int>(Points[0][0]);
	int p0_y = static_cast<int>(Points[0][1]);
	int p1_x = static_cast<int>(Points[1][0]);
	int p1_y = static_cast<int>(Points[1][1]);

	//draw a pixel with glVertex2i(x, y) or rather let bresenham do it:
	BresenhamLine(Point(p0_x, p0_y), Point(p1_x, p1_y), Color(0.7, 0.7, 0.0));

	glEnd();
}

void BresenhamLine(Point p1, Point p2, Color c)
{

	bool flipY = false;
	bool swapXY = false;

	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;

	if (p2.x < p1.x) std::swap(p1, p2); // immer von links nach rechts zeichnen

	if (dy < 0) {
		flipY = true;
		p1.y = -p1.y;
		p2.y = -p2.y;
		dy = -dy;
	}
	dx = p2.x - p1.x; dy = p2.y - p1.y;

	// falls steiler als 45°: x<->y tauschen
	if (dy > dx) {
		swapXY = true;
		std::swap(p1.x, p1.y);
		std::swap(p2.x, p2.y);
		// dx,dy werden gleich vor dem Kern nochmal frisch berechnet
	}


	// initializations

	int x, y, d, dne, de;
	x = p1.x;				y = p1.y;
	dx = p2.x - x;			dy = p2.y - y;
	dne = 2 * (dy - dx);	de = 2 * dy;
	d = 2 * dy - dx;


	// Rücktransformation (inverse Reihenfolge!) + Plot
	auto plot = [&](int px, int py) {
		if (swapXY) std::swap(px, py);  // zuerst y=x rückgängig
		if (flipY)  py = -py;           // dann x-Achse rückgängig
		glVertex2i(px, py);
		};

	// Startpunkt
	plot(x, y);

	// 1.-Oktant Bresenham
	while (x < p2.x) {
		if (d >= 0) { d += dne; x++; y++; }   // NE
		else { d += de;  x++; }  // E
		plot(x, y);
	}

	// last point
	plot(p2.x, p2.y);
}
