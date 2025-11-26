#pragma once
#include "Vector.h"
#include "Matrix.h"

class Scene
{
public:
	CVec4f projectZ(float fFocus, CVec4f pView);
	CVec3f* constructQuader(CVec3f center, float width, float height, float depth);
	void shiftQuader(CVec3f* quader, float x, float y, float z);
};

