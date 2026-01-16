/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 4 	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// 
// INCLUDES
// 
/////////////////////////////////////////////////////////////

// include-file for text in-/output
#include <iostream>
using namespace std;

// include-file for GLUT-Library
#include "glut.h"

// my own include-files
#include "Vector.h"
#include "Color.h"
#include "Sphere.h"
#include "cg_math.h"

/////////////////////////////////////////////////////////////
// 
// GLOBAL VARIABLES
// 
/////////////////////////////////////////////////////////////

// resolution of texture 
// !!!ATTENTION!!! not every resolution works!
#define TEX_RES_X 500
#define TEX_RES_Y 500
// Number of pixel of texture
#define TEX_RES TEX_RES_X*TEX_RES_Y
// length of texture's axis is asymmetric: -TexRes#/2 to TexRes#/2-1

// global texture buffer
char g_Buffer[3*TEX_RES];
// texture ID 
GLuint g_TexID = 0;

// resolution of main window (can be changed by user)
int g_WinWidth  = 1000;
int g_WinHeight = 1000;

// increments for interaction
double illum_inc     = 0.05;		// increment for illumination values
double color_inc     = 0.05;		// increment for color        values
double light_inc     = 0.1;			// increment for angles in spherical angles for light source position
double shininess_inc = 5.0;

// scene parameters
Sphere sphere(CVec3d(0, 0, 100), 50);
unsigned material = 0;				// default = 0, brass = 1, bronze = 2

// light source and camera parameters
int A = -1;							// eye-point
CVec3d affLight(   0,   1000, 0);	// affine    coordinates of light source, i.e. (x,y,z)
CVec3d sphLight(1000, 1.5*PI, 0);	// spherical coordinates of light source, i.e. (radius, phi, psi)
Color  backColor = Color(0.8,0.8,0.8);

// default illumination levels
CVec3d ambientIllumination (0.1, 0.1, 0.1);
CVec3d diffuseIllumination (1.0, 1.0, 1.0);
CVec3d specularIllumination(0.5, 0.5, 0.5);

// default reflection coefficients
Color  specularColor(0.5,0.5,0.5);	// specular color, aka color of light source
Color  ambientColor (0.5,0.5,0.5);	// ambient color , aka color of environment
Color  diffuseColor (1.0,1.0,1.0);	// diffuse color , aka color of object
double shininess = 28.0;			// shininess

// other reflection coefficients
// brass ( dt. Messing)
Color  ambientBrass   = Color(0.33,0.22,0.03);
Color  diffuseBrass   = Color(0.78,0.57,0.11);
Color  specularBrass  = Color(0.99,0.94,0.81);
double shininessBrass = 28.0;
// bronze
Color  ambientBronze   = Color(0.21,0.13,0.05);
Color  diffuseBronze   = Color(0.71,0.43,0.18);
Color  specularBronze  = Color(0.39,0.27,0.17);
double shininessBronze = 26.0;

// forward declarations
void clearTexture (                 const Color& c = backColor);
void setPixel     (const CVec2i& p, const Color& c = backColor);

/////////////////////////////////////////////////////////////
// 
// YOUR FUNCTIONS
// 
///////////////////////////////////////////////////////////// 

CVec3d intersectSphere(const CVec3d& EyePos, const CVec3d& ViewDir) 
{
	////////////////////////////////////////////////////////////	
	// 
	// This is the place to implement a sphere-ray-intersection
	// 
	////////////////////////////////////////////////////////////
	// EyePos + lambda*ViewDir is on the surface of the sphere
	
	
	const CVec3d L = EyePos - sphere.getCenter();

	const double a = ViewDir.dot(ViewDir);   // = 1 if normalized
	const double b = 2.0 * ViewDir.dot(L);
	const double c = L.dot(L) - sphere.getRadius() * sphere.getRadius();

	const double discriminant = b * b - 4 * a * c;

	if (discriminant < 0.0f)
		return CVec3d(0, 0, -1);

	const double sqrtD = std::sqrt(discriminant);

	double t0 = (-b - sqrtD) / (2 * a);
	double t1 = (-b + sqrtD) / (2 * a);

	// ensure t0 <= t1
	if (t0 > t1) std::swap(t0, t1);

	// t0 is the lambda for the first intersection point

	return EyePos + ViewDir * t0;
}

Color illumination(const CVec3d& HitPos, const CVec3d& ViewDir) 
{	
	// reflection coefficients
	// reflection coefficients
	Color  Ka, Kd, Ks;					// ambient, diffuse, specular material coefficients
	double shi;							// shininess
	switch (material) {
	default:
	case 0:	// default color
		Ka  = ambientColor;				// ambient   material coefficient
		Kd  = diffuseColor;				// diffuse   material coefficient
		Ks  = specularColor;			// specular  material coefficient
		shi = shininess;				// shininess material coefficient
		break;
	case 1:	// brass
		Ka  = ambientBrass;				// ambient   material coefficient
		Kd  = diffuseBrass;				// diffuse   material coefficient
		Ks  = specularBrass;			// specular  material coefficient
		shi = shininessBrass;			// shininess material coefficient
		break;
	case 2:	// bronze
		Ka  = ambientBronze;			// ambient   material coefficient
		Kd  = diffuseBronze;			// diffuse   material coefficient
		Ks  = specularBronze;			// specular  material coefficient
		shi = shininessBronze;			// shininess material coefficient
		break;
	}

	// illumination levels
	CVec3d Ia = ambientIllumination;	// ambient  illumination level
	CVec3d Id = diffuseIllumination;	// diffuse  illumination level
	CVec3d Is = specularIllumination;	// specular illumination level						

	////////////////////////////////////////////////////////////	
	// 
	// This is the place to implement Phong illumination
	// 
	CVec3d LightPos   = affLight;
	Color  pixelColor = backColor;
	// 
	// Please try different parameter settings for Ka, Ks, Kd, shi, Ia, Id, Is
	// 
	////////////////////////////////////////////////////////////



	//ambient component
	const Color ambientComponent = Ka * Ia;
	
	//diffuse component
	const CVec3d N = (HitPos - sphere.getCenter()).normalized(); //normal vector
	const CVec3d L = (LightPos - HitPos).normalized(); //light vector
	const double ndotl = max(0.0, N.dot(L));
	const Color diffuseComponent = Kd * Id * ndotl;

	//specular component
	const CVec3d EyePos = CVec3d(0.0, 0.0, A);
	const CVec3d V = (EyePos - HitPos).normalized();
	const CVec3d R = (N * (2.0 * N.dot(L)) - L).normalized();
	const double rdotv = max(0.0, R.dot(V));
	const Color specularComponent = Ks * Is * pow(rdotv, shi);

	double r = min(1.0, ambientComponent.r + diffuseComponent.r + specularComponent.r);
	double g = min(1.0, ambientComponent.g + diffuseComponent.g + specularComponent.g);
	double b = min(1.0, ambientComponent.b + diffuseComponent.b + specularComponent.b);
	pixelColor = Color(r, g, b);

	return pixelColor;
}

// This function controls and triggers the rendering process
void rayCast() 
{
	clearTexture(backColor);

	CVec3d eyePoint = CVec3d(0.0,0.0, A);	// eye-point
	CVec3d viewDir  = CVec3d(0.0,0.0,-A);	// view-direction
	CVec3d hitPoint;
	Color  pixelColor;

	// iterate over the pixels
	for (int x = 0; x < TEX_RES_X; x++) {
		for (int y = 0; y < TEX_RES_Y; y++) {
			// convert viewport to view window
			viewDir[0] = -1 + 2*x / static_cast<float>(TEX_RES_X -1);
			viewDir[1] = -1 + 2*y / static_cast<float>(TEX_RES_Y -1);

			// compute scene point covered by pixel
			hitPoint = intersectSphere(eyePoint,viewDir);

			// compute pixel color
			pixelColor = backColor; // background color
			if (hitPoint[2] != -1) pixelColor = illumination(hitPoint, viewDir);

			// draw the pixel
			setPixel(CVec2i(x, y), pixelColor);
		}
	}
	cout << "raycast done" << endl;
}


/////////////////////////////////////////////////////////////
// 
// PLEASE-DO-NOT-CHANGE FUNCTIONS
// 
///////////////////////////////////////////////////////////// 

// Functions to set up the texture.
// Please do not change!
void manageTexture () 
{
	glEnable (GL_TEXTURE_2D);

	if (g_TexID==0) glGenTextures(1, &g_TexID);

	glBindTexture  (GL_TEXTURE_2D , g_TexID);
	glTexEnvf      (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE  , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S    , GL_CLAMP  );
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T    , GL_CLAMP  );

	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, TEX_RES_X, TEX_RES_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, g_Buffer);	
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable    (GL_TEXTURE_2D);
}

// Functions to map the texture.
// Please do not change!
void mapTexture() 
{
	glBindTexture (GL_TEXTURE_2D, g_TexID);	
	glEnable      (GL_TEXTURE_2D);

	glBegin (GL_QUADS);
	glColor3d   (1.0, 0.0, 0.0);
	glTexCoord2d(0.0, 0.0);
	glVertex2d  (-g_WinWidth/2.0, -g_WinHeight/2.0);
	glTexCoord2d(1.0, 0.0);
	glVertex2d  ( g_WinWidth/2.0, -g_WinHeight/2.0);
	glTexCoord2d(1.0, 1.0);
	glVertex2d  ( g_WinWidth/2.0,  g_WinHeight/2.0);
	glTexCoord2d(0.0, 1.0);
	glVertex2d  (-g_WinWidth/2.0,  g_WinHeight/2.0);
	glEnd ();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable    (GL_TEXTURE_2D);
}

// Callback function to adjust the window size.
// Please do not change!
void reshape(int w, int h) 
{
	g_WinWidth = w;
	g_WinHeight = h;

	glViewport(0, 0, w, h);						// Establish viewport to cover entire window.

	glMatrixMode(GL_PROJECTION);				// Start modifying the projection matrix.
	glLoadIdentity();							// Reset projection matrix.
	glOrtho(-w/2, w/2, -h/2, h/2, 0, 1);		// Map abstract coords directly to window coords.

	glutPostRedisplay ();
}

// Function clears the texture and fills it with color c.
// Please do not change!
void clearTexture (const Color& c) 
{
	for (int i=0; i<TEX_RES; i++) {
		g_Buffer[3 * i    ] = char(255.0 * c.r);
		g_Buffer[3 * i + 1] = char(255.0 * c.g);
		g_Buffer[3 * i + 2] = char(255.0 * c.b);
	}
}

// Function plots a pixel p with color c to the texture. 
// Please do not change it, but use it!
void setPixel(const CVec2i& p, const Color& c) 
{
	if (p[0] < 0 || p[1] < 0 || p[0] > TEX_RES_X || p[1] > TEX_RES_Y) {
		cerr << "Illegal pixel co-ordinates (" << p[0] << ", " << p[1] << ")\n" << flush;
		return;
	}
	
	g_Buffer[3 * TO_LINEAR(p[0], p[1])    ] = char(255.0 * c.r);
	g_Buffer[3 * TO_LINEAR(p[0], p[1]) + 1] = char(255.0 * c.g);
	g_Buffer[3 * TO_LINEAR(p[0], p[1]) + 2] = char(255.0 * c.b);
}

// Callback function controlling the repaint of the display.
// Please do not change!
void display() 
{
	manageTexture  ();	
	glClear        (GL_COLOR_BUFFER_BIT);	
	mapTexture     ();
	glFlush        ();
	glutSwapBuffers();
}

// Callback function initializing the repaint of the display.
// Please do not change!
void init() 
{
	affLight = SphericalToAffine(sphLight);
	rayCast();
}

// Callback function to handle keyboard input.
// Please do not change!
void keyboard(unsigned char c, int x, int y) 
{
	CVec3d inc(illum_inc,illum_inc,illum_inc);
	switch (c) {

		/////////////////////////////////
		// Light source
		/////////////////////////////////
		case 'x': sphLight[1] = crop(PI, 2 * PI, sphLight[1] - light_inc); break;
		case 'X': sphLight[1] = crop(PI, 2 * PI, sphLight[1] + light_inc); break;
		case 'y': sphLight[2] = crop( 0,     PI, sphLight[2] - light_inc); break;
		case 'Y': sphLight[2] = crop( 0,     PI, sphLight[2] + light_inc); break;
		case 'z': sphLight[0] -= 50; break;
		case 'Z': sphLight[0] += 50; break;

		/////////////////////////////////
		// Color of sphere
		/////////////////////////////////
		case 'r': diffuseColor.r = crop(diffuseColor.r - color_inc); break;
		case 'R': diffuseColor.r = crop(diffuseColor.r + color_inc); break;
		case 'g': diffuseColor.g = crop(diffuseColor.g - color_inc); break;
		case 'G': diffuseColor.g = crop(diffuseColor.g + color_inc); break;
		case 'b': diffuseColor.b = crop(diffuseColor.b - color_inc); break;
		case 'B': diffuseColor.b = crop(diffuseColor.b + color_inc); break;

		/////////////////////////////////
		// Color of light source
		/////////////////////////////////
		case 'c': specularColor.r = crop(specularColor.r - color_inc); break;
		case 'C': specularColor.r = crop(specularColor.r + color_inc); break;
		case 'e': specularColor.g = crop(specularColor.g - color_inc); break;
		case 'E': specularColor.g = crop(specularColor.g + color_inc); break;
		case 'f': specularColor.b = crop(specularColor.b - color_inc); break;
		case 'F': specularColor.b = crop(specularColor.b + color_inc); break;

		/////////////////////////////////
		// Color of environment
		/////////////////////////////////
		case 'u': ambientColor.r = crop(ambientColor.r - color_inc); break;
		case 'U': ambientColor.r = crop(ambientColor.r + color_inc); break;
		case 'v': ambientColor.g = crop(ambientColor.g - color_inc); break;
		case 'V': ambientColor.g = crop(ambientColor.g + color_inc); break;
		case 'w': ambientColor.b = crop(ambientColor.b - color_inc); break;
		case 'W': ambientColor.b = crop(ambientColor.b + color_inc); break;

		/////////////////////////////////
		// Shininess
		/////////////////////////////////
		case 's': shininess -= min(shininess_inc, shininess / 2); shininess = max(0.0, shininess);  break;
		case 'S': shininess += min(shininess_inc, shininess); break;

		/////////////////////////////////
		// Illumination level
		/////////////////////////////////
		case '1': diffuseIllumination  = crop(diffuseIllumination  - inc); break;		// diffuse  illumination
		case '2': diffuseIllumination  = crop(diffuseIllumination  + inc); break;		// diffuse  illumination
		case '3': specularIllumination = crop(specularIllumination - inc); break;		// specular illumination
		case '4': specularIllumination = crop(specularIllumination + inc); break;		// specular illumination
		case '5': ambientIllumination  = crop(ambientIllumination  - inc); break;		// ambient  illumination
		case '6': ambientIllumination =  crop(ambientIllumination  + inc); break;		// ambient  illumination
		
		/////////////////////////////////
		// Radius
		/////////////////////////////////
		case 'd': sphere.setRadius(sphere.getRadius()-1); break;
		case 'D': sphere.setRadius(sphere.getRadius()+1); break;

		/////////////////////////////////
		// Material properties of sphere
		/////////////////////////////////
		case 'M': material = (material+1)%3; break;

		/////////////////////////////////
		// Quit
		/////////////////////////////////
		case 'q':
		case 'Q': exit (0); break;

		/////////////////////////////////
		// Help menue
		/////////////////////////////////
		default:	
			cout << "Use 'x', 'X', 'y', and 'Y'           to move   the light source"						<< endl; 
			cout << "Use 'r', 'R', 'g', 'G', 'b', and 'B' to change the diffuse  color"						<< endl; 
			cout << "Use 'c', 'C', 'e', 'E', 'f', and 'F' to change the specular color"						<< endl;
			cout << "Use 'u', 'U', 'v', 'V', 'w', and 'W' to change the ambient  color"						<< endl; 
			cout << "Use '1' and '2'                      to change the diffuse  illumination level"		<< endl; 
			cout << "Use '3' and '4'                      to change the specular illumination level"		<< endl;
			cout << "Use '5' and '6'                      to change the ambient  illumination level"		<< endl; 
			cout << "Use 's' and 'S'                      to change the shininess"							<< endl; 
			cout << "Use 'd' and 'D'                      to change the radius"								<< endl; 
			cout << "Use 'M'                              to change the material (normal/Messing/Bronze)"	<< endl; 
			cout << "Use 'q' or 'Q'                       to quit"											<< endl; 
			break;
	}
	cout << "Diffuse  color and illumination level: "	<< diffuseColor  << ", " << diffuseIllumination		<< endl;
	cout << "Specular color and illumination level: "	<< specularColor << ", " << specularIllumination	<< endl;
	cout << "Ambient  color and illumination level: "	<< ambientColor  << ", " << ambientIllumination		<< endl;
	cout << "Shininess :"								<< shininess										<< endl;
	cout << "Material: "								<< material											<< endl;

	affLight=SphericalToAffine(sphLight);
	rayCast();	
	glutPostRedisplay();
}

// This is the main function, triggering the GLUT-Main-Loop
// Please do not change!
int main (int argc, char **argv) {
	
	init();

	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize (g_WinWidth, g_WinHeight);

	glutCreateWindow ("Assignment 4: Phong Illumination (WS25/26)");
	
	glutKeyboardFunc(keyboard);		// handles keyboard input
	glutReshapeFunc (reshape);		// handles changes of window size
	glutDisplayFunc (display);		// handles repainting of the display
	
	glutMainLoop ();				// transfering the control to GLUT

	glDeleteTextures (1, &g_TexID);	// deletes the defined textures

	return 0;
}
