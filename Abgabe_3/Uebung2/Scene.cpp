#include "Scene.h"

// From the assignment description:
//- the view-origin ViewOrigin is the origin of the view-coordinate system,
//- the view-direction ViewDir is anti-parallel to the die zzvv-axis (and initially also the zzww-axis),
//- the view-up-vector ViewUp (yyvv-axis of the image plane) is initially parallel to the yyww-axis,
//- the eye-point EyePoint is on the positive zzvv-axis, i.e. (0,0,fFocus) in view-coordinates,
//- the image-plane is the xxvv yvv-plane,
//- the focal-distance fFocus is the distance of the eye-point to the view-origin, and
//- the focal-distance fFocus is a variable parameter of the function projectZ.

CVec4f Scene::projectZ(float fFocus, CVec4f pView)
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

CVec3f* Scene::constructQuader(CVec3f center, float width, float height, float depth)
{
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

void Scene::shiftQuader(CVec3f* quader, float x, float y, float z)
{
	for (int i = 0; i < 8; i++)
	{
		quader[i][0] += x;
		quader[i][1] += y;
		quader[i][2] += z;
	}
}
