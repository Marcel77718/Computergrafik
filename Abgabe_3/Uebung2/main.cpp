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
float center1[4] = { 0, 0, 0, 1 };
CVec4f center_1 = CVec4f(center1);
CVec3f* quader1 = constructQuader(center_1, 200, 200, 200);
float center2[4] = { -700, -400, -200, 1 };
CVec4f center_2 = CVec4f(center2);
CVec3f* quader2 = constructQuader(center_2, 100, 500, 200);
float center3[4] = { 600, -20, -250, 1.0 };
CVec4f center_3 = CVec4f(center3);
CVec3f* quader3 = constructQuader(center_3, 200, 400, 500);

float center2_mir[4] = { 700, -400, -200, 1 };
CVec4f center_2_mir = CVec4f(center2_mir);
CVec3f* quader2_mir = constructQuader(center_2_mir, 100, 500, 200);


CVec4f ViewOrigin;
CVec4f ViewDir;
CVec4f ViewUp;
float fFocus;

//
/////////////////////////////////////////////////////////////

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
	/*std::cout << "Transformation Matrix: " << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << transMat(i, j) << " ";
		}
		std::cout << "\n\n" << std::endl;
	}*/
	
	CVec3f cuboid_in_view_coords[8];
	/*for (int i = 0; i < 8; i++)
	{
		cuboid_in_view_coords[i] = (CVec3f)projectZallg(transMat, fFocus, CVec4f(quader1[i], 1.0f));
	}*/
	drawQuader(cuboid_in_view_coords, fFocus, SunYellow, transMat);

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
			fFocus += 1000.0f;
			glutPostRedisplay();
			break;
		case 'f': // decrease focal distance
			fFocus -= 1000.0f;
			glutPostRedisplay();
			break;
		case 'X' : // increase x-rotation around WC

			break;
		case 'Y' : // increase y-rotation around WC
			break;
		case 'Z' : // increase z-rotation around WC
			break;
		case 'x' : // decrease x-rotation around WC
			break;
		case 'y' : // decrease y-rotation around WC
			break;
		case 'z' : // decrease z-rotation around WC
			break;	
		case 'A': // increase x-rotation around VC
			break;
		case 'B': // increase y-rotation around VC
			break;
		case 'C': // increase z-rotation around VC
			break;
		case 'a': // decrease x-rotation around VC
			break;
		case 'b': // decrease y-rotation around VC
			break;
		case 'c' : // decrease z-rotation around VC
			break;
		case 'U': // translate along x in WC
			ViewOrigin[0] += 50.0f;
			break;
		case 'V': // translate along y in WC
			ViewOrigin[1] += 50.0f;
			break;
		case 'W': // translate along z in WC
			ViewOrigin[2] += 50.0f;
			break;
		case 'u': // translate along -x in WC
			ViewOrigin[0] -= 50.0f;
			break;
		case 'v': // translate along -y in WC
			ViewOrigin[1] -= 50.0f;
			break;
		case 'w': // translate along -z in WC
			ViewOrigin[2] -= 50.0f;
			break;
		case 'R': // Reset view
			ViewOrigin = CVec4f(CVec3f(0.0f, 0.0f, 0.0f), 1.0f);
			ViewDir = CVec4f(CVec3f(0.0f, 0.0f, -1.0f), 0.0f);
			ViewUp = CVec4f(CVec4f(0.0f, 1.0f, 0.0f), 0.0f);
			fFocus = 500.0f;
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
