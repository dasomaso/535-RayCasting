/***************************************************
*  This program draws an American national flag.
*  Click the right button to exit.
**************************************************/

//cslab version
//#include <X11/Xlib.h>
//#include <GLUT/glut.h>
//#include <OpenGL/gl.h>

//cs domain version
//#include <X11/Xlib.h>
//#include <GL/glut.h>
//#include <GL/gl.h>

//windows version
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <list>

#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>
#include <time.h>
#include <limits>

#include "Solid.h"
#include "Box.h"
#include "Vector3.h"
#include "Ray.h"

const double PI = 3.1415926;
const int MAX_RECURSION_DEPTH = 5;
const float VIEWPORT_X = 6;
const float VIEWPORT_Y = 4.5;
const float FOCAL_LENGTH = -0.5;
const int screenWidth = 640;
const int screenHeight = 480;

//unsigned int*** pixelData;
GLubyte pixelData[screenHeight][screenWidth][3];

Vector3* light;

std::list<Solid*>* objectList;

Ray* camera = new Ray(new Vector3(0, 0, 3.0), new Vector3(0.0, 0.0, -1.0));
Vector3* urCorner = new Vector3(VIEWPORT_X / 2, VIEWPORT_Y / 2, FOCAL_LENGTH);
Vector3* blCorner = new Vector3(-1 * VIEWPORT_X / 2, -1 * VIEWPORT_Y / 2, FOCAL_LENGTH);
Vector3* s;
Ray* cast;

Solid::Intercept* raycast(Solid* obj, Ray* r, int depth)
{
	if (depth == 0) { return NULL; }

	Solid::Intercept hit = obj->FindRayIntersect(*(r));

	depth--;
	if (hit.t != std::numeric_limits<float>::infinity()) {
		return &hit;
	}
	else {
		return NULL;
	}
}

void renderScene()
{
	//Iterate through all screen pixels
	for (int i = 0; i < screenWidth; i++) {
		for (int j = 0; j < screenHeight; j++) {

			//Set background color
			pixelData[j][i][0] = 25;
			pixelData[j][i][1] = 25;
			pixelData[j][i][2] = 25;

			//Construct viewing ray
			s = new Vector3(
				blCorner->x + (urCorner->x - blCorner->x)*((i + 0.5) / viewport[2]),
				blCorner->y + (urCorner->y - blCorner->y)*((j + 0.5) / viewport[3]),
				urCorner->z
				);

			cast = new Ray();
			*(cast->start) = *(camera->start);
			*(cast->direction) = *(s->subtract(camera->start));

			//Check for intersection with objects in scene
			std::list<Solid*>::const_iterator objIterator;
			Solid::Intercept* result = new Solid::Intercept();
			result->mat.color[0] = 25;
			result->mat.color[1] = 25;
			result->mat.color[2] = 25;
			result->normal = new Vector3();
			result->t = std::numeric_limits<float>::infinity();

			for (objIterator = objectList->begin(); objIterator != objectList->end(); objIterator++)
			{
				Solid::Intercept* hit = raycast(*objIterator, cast, MAX_RECURSION_DEPTH);
				if (hit && hit->t < result->t) {
					result->mat = hit->mat;
					result->normal = hit->normal;
					result->t = hit->t;
				}
			}

			//If an object was hit, set the color for that pixel appropriately
			pixelData[j][i][0] = result->mat.color[0];
			pixelData[j][i][1] = result->mat.color[1];
			pixelData[j][i][2] = result->mat.color[2];
		}
	}

	glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
}

void constructScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // clear the screen

	light = new Vector3(2.0, 6.0, 3.0);

	objectList = new std::list<Solid*>();

	Box* floor = new Box();
	floor->scale = new Vector3(1.5, 0.05, 1.5);
	floor->position = new Vector3(0.0, -0.5, 0.0);
	floor->mat.color[0] = 178;
	floor->mat.color[1] = 0;
	floor->mat.color[2] = 0;
	objectList->push_back(floor);

	Box* wall1 = new Box();
	wall1->scale = new Vector3(0.05, 1.0, 1.5);
	wall1->position = new Vector3(-0.75, 0.0, 0.0);
	wall1->mat.color[0] = 178;
	wall1->mat.color[1] = 178;
	wall1->mat.color[2] = 178;
	objectList->push_back(wall1);

	Box* wall2 = new Box();
	wall2->scale = new Vector3(1.5, 1.0, 0.05);
	wall2->position = new Vector3(0.0, 0.0, -0.75);
	wall2->mat.color[0] = 178;
	wall2->mat.color[1] = 178;
	wall2->mat.color[2] = 178;
	objectList->push_back(wall2);
}

void myDisplay(void)
{
	constructScene();

	renderScene();
	
	glFlush();

	//glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			exit(-1);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Shaded example - 3D scene");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);                 // register myMouse function
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);  // background is light gray
	glViewport(0, 0, 640, 480);
	glutMainLoop();
}
