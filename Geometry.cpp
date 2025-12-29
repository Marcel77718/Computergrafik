#include "Geometry.h"


// From the assignment description:
//- the view-origin ViewOrigin is the origin of the view-coordinate system,
//- the view-direction ViewDir is anti-parallel to the die zzvv-axis (and initially also the zzww-axis),
//- the view-up-vector ViewUp (yyvv-axis of the image plane) is initially parallel to the yyww-axis,
//- the eye-point EyePoint is on the positive zzvv-axis, i.e. (0,0,fFocus) in view-coordinates,
//- the image-plane is the xxvv yvv-plane,
//- the focal-distance fFocus is the distance of the eye-point to the view-origin, and
//- the focal-distance fFocus is a variable parameter of the function projectZ.

bool first_round = true; //debugging variable

CVec4f projectZ(float fFocus, CVec4f pView)
{
    float denom = fFocus - pView[2];
    
	if (denom <= 1e-3f) // Point is behind or too close to the eye point
    {
		return CVec4f(CVec3f(1e9f, 1e9f, 1e9f), 0.0f); // Return invalid point
	}

    //the real projection
	float x_proj = (fFocus / denom) * pView[0];
	float y_proj = (fFocus / denom) * pView[1];
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
	CVec3f* vertices = new CVec3f[8]; //indices are connected so that the same modulo(4)s are connected to each other and ajacent indices as well.
	// However, there is no connection between index 3 and 4, beware!
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

CVec3f* give_flat_quader()
{
    CVec3f* flat = new CVec3f[8];
    flat[0] = CVec3f(10, 0, 0);
    flat[1] = CVec3f(10, 10, 0);
    flat[2] = CVec3f(0, 10, 0);
	flat[3] = CVec3f(0, 0, 0);
	flat[4] = CVec3f(20, 0, 0);
	flat[5] = CVec3f(20, 20, 0);
	flat[6] = CVec3f(0, 20, 0);
	flat[7] = CVec3f(0, 0, 0);
    return flat;
}

// Helper function to check if a projected point is valid
bool isValidPoint(const CVec3f& p)
{
	// Reject points with very large coordinates (invalid or too far off screen)
	return std::abs(p[0]) < 5000.0f && std::abs(p[1]) < 5000.0f;
}

void drawProjektedZ(CVec3f Points[8])
{
	int p0_x = static_cast<int>(Points[0][0]);
	int p0_y = static_cast<int>(Points[0][1]);
	int p1_x = static_cast<int>(Points[1][0]);
	int p1_y = static_cast<int>(Points[1][1]);

	//draw a pixel with glVertex2i(x, y) or rather let bresenham do it:
	//BresenhamLine(Point(p0_x, p0_y), Point(p1_x, p1_y), Color(0.7, 0.7, 0.0));
	// 
	// first square
	for (int i = 0; i < 3; i++)
	{
		if (!isValidPoint(Points[i]) || !isValidPoint(Points[i + 1])) continue; // Skip invalid points
		int pA_x = static_cast<int>(Points[i][0]);
		int pA_y = static_cast<int>(Points[i][1]);
		int pB_x = static_cast<int>(Points[i + 1][0]);
		int pB_y = static_cast<int>(Points[i + 1][1]);
		BresenhamLine(Point(pA_x, pA_y), Point(pB_x, pB_y), Color(0.7, 0.7, 0.0));
	}
	//closing first square
	if (isValidPoint(Points[3]) && isValidPoint(Points[0])) {
		int p3_x = static_cast<int>(Points[3][0]);
		int p3_y = static_cast<int>(Points[3][1]);
		BresenhamLine(Point(p3_x, p3_y), Point(p0_x, p0_y), Color(0.7, 0.7, 0.0));
	}

	//second square
	for (int i = 4; i < 7; i++)
	{
		if (!isValidPoint(Points[i]) || !isValidPoint(Points[i + 1])) continue; // Skip invalid points
		int pA_x = static_cast<int>(Points[i][0]);
		int pA_y = static_cast<int>(Points[i][1]);
		int pB_x = static_cast<int>(Points[i + 1][0]);
		int pB_y = static_cast<int>(Points[i + 1][1]);
		BresenhamLine(Point(pA_x, pA_y), Point(pB_x, pB_y), Color(0.7, 0.7, 0.0));
	}
	//closing second square
	if (isValidPoint(Points[7]) && isValidPoint(Points[4])) {
		int p7_x = static_cast<int>(Points[7][0]);
		int p7_y = static_cast<int>(Points[7][1]);
		int p4_x = static_cast<int>(Points[4][0]);
		int p4_y = static_cast<int>(Points[4][1]);
		BresenhamLine(Point(p7_x, p7_y), Point(p4_x, p4_y), Color(0.7, 0.7, 0.0));
	}

	
	//connecting lines
	for (int i = 0; i < 4; i++)
	{
		if (!isValidPoint(Points[i]) || !isValidPoint(Points[i + 4])) continue; // Skip invalid points
		int pA_x = static_cast<int>(Points[i][0]);
		int pA_y = static_cast<int>(Points[i][1]);
		int pB_x = static_cast<int>(Points[i + 4][0]);
		int pB_y = static_cast<int>(Points[i + 4][1]);
		if (first_round)
		{
			std::cout << "Point A: (" << pA_x << ", " << pA_y << ")\n";
			std::cout << "Point B: (" << pB_x << ", " << pB_y << ")\n";
		}
		BresenhamLine(Point(pA_x, pA_y), Point(pB_x, pB_y), Color(0.7, 0.7, 0.0));
	}
	first_round = false;

}

void BresenhamLine(Point p1, Point p2, Color c)
{
	// Classic "all-octants" Bresenham (integer), robust for every direction.
	// IMPORTANT: glBegin(...) / glEnd() must be outside, like you already do.

	int x1 = p1.x, y1 = p1.y;
	int x2 = p2.x, y2 = p2.y;

	int dx = std::abs(x2 - x1);
	int dy = std::abs(y2 - y1);

	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;

	int err = dx - dy; // error term

	while (true)
	{
		glVertex2i(x1, y1);

		if (x1 == x2 && y1 == y2)
			break;

		int e2 = 2 * err;

		// move in x?
		if (e2 > -dy)
		{
			err -= dy;
			x1 += sx;
		}

		// move in y?
		if (e2 < dx)
		{
			err += dx;
			y1 += sy;
		}
	}
}

void drawQuader(CVec3f Cuboid[8], float fFocus, Color c)
{
	glBegin(GL_POINTS);
	glColor3d(c.r, c.g, c.b);

	CVec3f projectedQuader[8];
	for (int i = 0; i < 8; i++)
	{
		projectedQuader[i] = projectZ(fFocus, CVec4f(Cuboid[i], 1.0f));
	}

	drawProjektedZ(projectedQuader);
	glEnd();
}

void drawQuader(CVec3f Cuboid[8], float fFocus, Color c, CMat4f matTransf)
{
	glBegin(GL_POINTS);
	glColor3d(c.r, c.g, c.b);
	CVec3f projectedQuader[8];
	for (int i = 0; i < 8; i++)
	{
		CVec4f pWorld = CVec4f(Cuboid[i], 1.0f);
		CVec4f pView = matTransf * pWorld;
		projectedQuader[i] = projectZallg(matTransf, fFocus, CVec4f(Cuboid[i], 1.0f));
	}
	drawProjektedZ(projectedQuader);
	glEnd();
}

CVec4f projectZallg(CMat4f matTransf, float fFocus, CVec4f pWorld)
{
	CVec4f pView = matTransf * pWorld;
	return projectZ(fFocus, pView);
}
