/////////////////////////////////////////////////////////////////////////////
//																		   //
// Code framework for lecture computer graphics WS 2025/26 assignment 2+3  //
//																		   //
// A generic class for n-dimensional coordinate vectors					   //
//																		   //
// (c) Georg Umlauf														   //
//																		   //
/////////////////////////////////////////////////////////////////////////////


#include "Vector.h"
#include "Matrix.h"

#include <iostream>
#include <cmath>

// maybe you have to switch to your local include path, depending on your GLUT installation
#include "glut.h"
// additions:

#include "Scene.h"

////////////////////////////////////////////////////////////
//
// system relevant global variables
//

// resolution of the of the main window (can be changed by the user)
int g_WinWidth  = 1600;
int g_WinHeight = 900;

// global variable to tune the timer interval
int g_iTimerMSecs;

//
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// private, global variables ... REPLACE BY YOUR OWN ONES
//
// some global state variables used to describe ...
float g_iPos;			// ... position and ...
float g_iPosIncr;		// ... position increment (used in display1)

CVec2i g_vecPos;		// same as above but in vector form ...
CVec2i g_vecPosIncr;	// (used in display2)


//our variables for assignment 3
float center1[4] = { 100, 100, 100, 1 };
CVec4f center_1 = CVec4f(center1);
CVec3f* quader1 = constructQuader(center_1, 200, 200, 200);

float center2[4] = { -700, -400, -200, 1 };
CVec4f center_2 = CVec4f(center2);
CVec3f* quader2 = constructQuader(center_2, 100, 500, 200);

float center3[4] = { 600, -20, -250, 1.0 };
CVec4f center_3 = CVec4f(center3);
CVec3f* quader3 = constructQuader(center_3, 200, 400, 500);

float center2_mir_arr[4] = { 700, -400, -200, 1 };
CVec4f center_2_mir = CVec4f(center2_mir_arr);
CVec3f* quader2_mir = constructQuader(center_2_mir, 100, 500, 200);


CVec4f ViewOrigin;
CVec4f ViewDir;
CVec4f ViewUp;
float fFocus;

//
/////////////////////////////////////////////////////////////

// Helper function: Rotate a vector around X-axis in world coordinates
CVec4f rotateAroundWorldX(const CVec4f& vec, float angle)
{
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	CVec4f result;
	result[0] = vec[0];
	result[1] = vec[1] * cos_a - vec[2] * sin_a;
	result[2] = vec[1] * sin_a + vec[2] * cos_a;
	result[3] = vec[3];
	return result;
}

// Helper function: Rotate a vector around Y-axis in world coordinates
CVec4f rotateAroundWorldY(const CVec4f& vec, float angle)
{
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	CVec4f result;
	result[0] = vec[0] * cos_a + vec[2] * sin_a;
	result[1] = vec[1];
	result[2] = -vec[0] * sin_a + vec[2] * cos_a;
	result[3] = vec[3];
	return result;
}

// Helper function: Rotate a vector around Z-axis in world coordinates
CVec4f rotateAroundWorldZ(const CVec4f& vec, float angle)
{
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	CVec4f result;
	result[0] = vec[0] * cos_a - vec[1] * sin_a;
	result[1] = vec[0] * sin_a + vec[1] * cos_a;
	result[2] = vec[2];
	result[3] = vec[3];
	return result;
}

// Helper function: Rotate a vector around an arbitrary axis in view coordinates
CVec4f rotateAroundViewAxis(const CVec4f& vec, const CVec4f& axis, float angle)
{
	// Rodrigues' rotation formula
	float cos_a = cos(angle);
	float sin_a = sin(angle);
	float one_minus_cos = 1.0f - cos_a;
	
	CVec4f normalized_axis = axis;
	float length = sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
	if (length > 1e-6f)
	{
		normalized_axis[0] /= length;
		normalized_axis[1] /= length;
		normalized_axis[2] /= length;
	}
	
	CVec4f result;
	// v_rot = v*cos(a) + (k x v)*sin(a) + k*(k·v)*(1-cos(a))
	float dot_product = normalized_axis[0]*vec[0] + normalized_axis[1]*vec[1] + normalized_axis[2]*vec[2];
	
	// Cross product k x v
	float cross_x = normalized_axis[1]*vec[2] - normalized_axis[2]*vec[1];
	float cross_y = normalized_axis[2]*vec[0] - normalized_axis[0]*vec[2];
	float cross_z = normalized_axis[0]*vec[1] - normalized_axis[1]*vec[0];
	
	result[0] = vec[0]*cos_a + cross_x*sin_a + normalized_axis[0]*dot_product*one_minus_cos;
	result[1] = vec[1]*cos_a + cross_y*sin_a + normalized_axis[1]*dot_product*one_minus_cos;
	result[2] = vec[2]*cos_a + cross_z*sin_a + normalized_axis[2]*dot_product*one_minus_cos;
	result[3] = vec[3];
	return result;
}

// Function to initialize our own variables
void init () 
{
	// init timer interval
	g_iTimerMSecs = 10;

	// init variables for display1
	g_iPos     = 0;
	g_iPosIncr = 2;



	// init variables for display2
	
	ViewOrigin = CVec4f(CVec4f(0.0f, 0.0f, 0.0f), 1.0f);
	ViewDir = CVec4f(CVec4f(0.0f, 0.0f, -1.0f), 0.0f);
	ViewUp = CVec4f(CVec4f(0.0f, 1.0f, 0.0f), 0.0f);
	fFocus = 500.0f;
}

// Function to initialize the view to ortho-projection.
// Please do not change.
void initGL () 
{
	glViewport (0, 0, g_WinWidth, g_WinHeight);		// Establish viewport to cover entire window.

	glMatrixMode (GL_PROJECTION);					// Start modifying the projection matrix.
	glLoadIdentity ();								// Init projection matrix.
	glOrtho (-g_WinWidth  /2, g_WinWidth  /2,		// Map abstract coords directly to window coords.
			 -g_WinHeight /2, g_WinHeight /2, 0, 1);	
	glDrawBuffer (GL_BACK);							// Tell GL that we draw to the back buffer and swap ...
													// ... buffers when image is ready to avoid flickering	
	float b = 70.0f / 100.0f;
	//glClearColor(b, b, b, 1.0f);					// Tell which color to use to clear image
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// black background
}

// Callback Function to change the window size.
// Please do not change.
void reshape(int w, int h)
{
	g_WinWidth  = w;
	g_WinHeight = h;
	glViewport(0, 0, w, h);							// Establish viewport to cover entire window.

	glMatrixMode(GL_PROJECTION);					// Start modifying the projection matrix.
	glLoadIdentity();								// Reset projection matrix.
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, 0, 1);	// Map abstract coords directly to window coords.

	glutPostRedisplay();
}

// timer callback function
void timer (int value) 
{
	///////
	// UPDATE YOUR VARIABLES HERE ...
	//


	//
	///////

	// These variables are just for the heaving triangles
	int size2 = std::min (g_WinWidth, g_WinHeight) / 2;

	// variables for displayExercise3 ... REPLACE BY YOUR OWN ONES
	if (g_iPos<=-size2 || g_iPos>=size2) g_iPosIncr = -g_iPosIncr;
	g_iPos += g_iPosIncr;

	// variables for displayExercise4 ... REPLACE BY YOUR OWN ONES
	if (g_vecPos[1]<=-size2 || g_vecPos[1]>=size2) g_vecPosIncr = -g_vecPosIncr; 
	g_vecPos += g_vecPosIncr;

	// the last two lines should always be
	glutPostRedisplay ();
	glutTimerFunc (g_iTimerMSecs, timer, 0);	// call timer for next iteration
}

// display callback function for EXERCISE 3
void displayExercise3(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	drawQuader(quader1, 500.0f, QuaderColor);
	drawQuader(quader2, 400.0f, Yellow);
	drawQuader(quader3, 300.0f, EarthBlue);

	

	// In double buffer mode the last two lines should always be
	glFlush        ();
	glutSwapBuffers(); // swap front and back buffer
}

// display callback function for EXERCISE 4
void displayExercise4(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

	CMat4f transMat = getTransform(ViewOrigin, ViewDir, ViewUp);
	
	CVec3f projectedQuader1[8];
	CVec3f projectedQuader2[8];
	CVec3f projectedQuader3[8];
	
	for (int i = 0; i < 8; i++)
	{
		projectedQuader1[i] = (CVec3f)projectZallg(transMat, fFocus, CVec4f(quader1[i], 1.0f));
		projectedQuader2[i] = (CVec3f)projectZallg(transMat, fFocus, CVec4f(quader2[i], 1.0f));
		projectedQuader3[i] = (CVec3f)projectZallg(transMat, fFocus, CVec4f(quader3[i], 1.0f));
	}
	
	drawQuader(projectedQuader1, fFocus, QuaderColor, transMat);
	drawQuader(projectedQuader2, fFocus, Yellow, transMat);
	drawQuader(projectedQuader3, fFocus, EarthBlue, transMat);

	// In double buffer mode the last two lines should always be
	glFlush        ();
	glutSwapBuffers(); // swap front and back buffer
}

void keyboard (unsigned char key, int x, int y) 
{
	switch (key) {
		case 'q':
		case 'Q':
			exit (0); // quit program
			break;
		case '1':
			glutDisplayFunc (displayExercise3);
			break;
		case '2':
			glutDisplayFunc (displayExercise4);
			break;
		case 'F': // increase focal distance
			fFocus += 50.0f;
			glutPostRedisplay();
			break;
		case 'f': // decrease focal distance
			fFocus -= 50.0f;
			if (fFocus < 1.0f) fFocus = 1.0f; // prevent negative focal distance
			glutPostRedisplay();
			break;
		case 'X': { // increase x-rotation around WC
			ViewDir = rotateAroundWorldX(ViewDir, 0.05f);
			ViewUp = rotateAroundWorldX(ViewUp, 0.05f);
			ViewOrigin = rotateAroundWorldX(ViewOrigin, 0.05f);

			glutPostRedisplay();
			break;
		}
		case 'Y': { // increase y-rotation around WC
			ViewDir = rotateAroundWorldY(ViewDir, 0.05f);
			ViewUp = rotateAroundWorldY(ViewUp, 0.05f);
			ViewOrigin = rotateAroundWorldY(ViewOrigin, 0.05f);

			glutPostRedisplay();
			break;
		}
		case 'Z': { // increase z-rotation around WC
			ViewDir = rotateAroundWorldZ(ViewDir, 0.05f);
			ViewUp = rotateAroundWorldZ(ViewUp, 0.05f);
			ViewOrigin = rotateAroundWorldZ(ViewOrigin, 0.05f);
			glutPostRedisplay();
			break;
		}
		case 'x': { // decrease x-rotation around WC
			ViewDir = rotateAroundWorldX(ViewDir, -0.05f);
			ViewUp = rotateAroundWorldX(ViewUp, -0.05f);
			ViewOrigin = rotateAroundWorldX(ViewOrigin, -0.05f);
			glutPostRedisplay();
			break;
		}
		case 'y': { // decrease y-rotation around WC
			ViewDir = rotateAroundWorldY(ViewDir, -0.05f);
			ViewUp = rotateAroundWorldY(ViewUp, -0.05f);
			ViewOrigin = rotateAroundWorldY(ViewOrigin, -0.05f);

			glutPostRedisplay();
			break;
		}
		case 'z': { // decrease z-rotation around WC
			ViewDir = rotateAroundWorldZ(ViewDir, -0.05f);
			ViewUp = rotateAroundWorldZ(ViewUp, -0.05f);
			ViewOrigin = rotateAroundWorldZ(ViewOrigin, -0.05f);

			glutPostRedisplay();
			break;
		}
		case 'A': { // increase roll VC (around ViewDir axis)
			ViewUp = rotateAroundViewAxis(ViewUp, ViewDir, 0.05f);
			glutPostRedisplay();
			break;
		}
		case 'B': { // increase yaw around VC (around right axis)
			ViewDir = rotateAroundViewAxis(ViewDir, ViewUp, 0.05f);
			glutPostRedisplay();
			break;
		}
		case 'C': { // increase pitch around VC (around right axis)
			float right[4] = { ViewUp[1] * ViewDir[2] - ViewUp[2] * ViewDir[1],
				ViewDir[0] * ViewUp[2] - ViewDir[2] * ViewUp[0],
				ViewUp[0] * ViewDir[1] - ViewUp[1] * ViewDir[0],
				0.0f };
			CVec4f rightVec = CVec4f(right);
			ViewDir = rotateAroundViewAxis(ViewDir, rightVec, 0.05f);
			ViewUp = rotateAroundViewAxis(ViewUp, rightVec, 0.05f);
			glutPostRedisplay();
			break;
		}
		case 'a': { // decrease roll around VC
			ViewUp = rotateAroundViewAxis(ViewUp, ViewDir, -0.05f);
			glutPostRedisplay();
			break;
		}
		case 'b': { // decrease yaw around VC
			
			ViewDir = rotateAroundViewAxis(ViewDir, ViewUp, -0.05f);
			glutPostRedisplay();
			break;
		}
		case 'c': { // decrease pitch around VC
			float right[4] = { ViewUp[1] * ViewDir[2] - ViewUp[2] * ViewDir[1],
				ViewDir[0] * ViewUp[2] - ViewDir[2] * ViewUp[0],
				ViewUp[0] * ViewDir[1] - ViewUp[1] * ViewDir[0],
				0.0f };
			CVec4f rightVec = CVec4f(right);
			ViewDir = rotateAroundViewAxis(ViewDir, rightVec, -0.05f);
			ViewUp = rotateAroundViewAxis(ViewUp, rightVec, -0.05f);
			glutPostRedisplay();
			break;
		}
		case 'U': // translate along x in WC
			ViewOrigin[0] += 50.0f;
			glutPostRedisplay();
			break;
		case 'V': // translate along y in WC
			ViewOrigin[1] += 50.0f;
			glutPostRedisplay();
			break;
		case 'W': // translate along z in WC
			ViewOrigin[2] += 50.0f;
			glutPostRedisplay();
			break;
		case 'u': // translate along -x in WC
			ViewOrigin[0] -= 50.0f;
			glutPostRedisplay();
			break;
		case 'v': // translate along -y in WC
			ViewOrigin[1] -= 50.0f;
			glutPostRedisplay();
			break;
		case 'w': // translate along -z in WC
			ViewOrigin[2] -= 50.0f;
			glutPostRedisplay();
			break;
		case 'R': // Reset view
			ViewOrigin = CVec4f(CVec3f(0.0f, 0.0f, 0.0f), 1.0f);
			ViewDir = CVec4f(CVec3f(0.0f, 0.0f, -1.0f), 0.0f);
			ViewUp = CVec4f(CVec3f(0.0f, 1.0f, 0.0f), 0.0f);
			fFocus = 500.0f;
			glutPostRedisplay();
			break;
		default:
			// do nothing ...
			break;
	};
}

// The main
// Please do not change.
int main (int argc, char **argv) 
{
	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutCreateWindow("Computer graphics assignment 2: 2d-Rotations (WS25/26)");
	glutReshapeWindow(g_WinWidth, g_WinHeight);

	init  ();	// init my variables first
	initGL();	// init the GL (i.e. view settings, ...)

	glutTimerFunc(10, timer, 0);
	glutReshapeFunc (reshape);			// is triggered on window size changes
	glutDisplayFunc (displayExercise3);	// is triggered to redraw the viewport/display
	glutKeyboardFunc(keyboard);			// is triggered on keyboard events

	// start main loop
	glutMainLoop ();

	return 0;
}
