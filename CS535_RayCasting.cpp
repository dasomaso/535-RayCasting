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
const int MAX_RECURSION_DEPTH = 3;
const float VIEWPORT_X = 4;
const float VIEWPORT_Y = 3;
const float FOCAL_LENGTH = -0.5;
const int screenWidth = 640;
const int screenHeight = 480;

GLubyte pixelData[screenHeight][screenWidth][3];

Vector3* light;
GLubyte lightColor[3];
GLubyte ambientLight[3];

std::list<Solid*>* objectList;

Ray* camera = new Ray(new Vector3(0.0, 0.0, 3.0), new Vector3(0.0, 0.0, -1.0));
Vector3* urCorner = new Vector3(VIEWPORT_X / 2, VIEWPORT_Y / 2, FOCAL_LENGTH);
Vector3* blCorner = new Vector3(-1 * VIEWPORT_X / 2, -1 * VIEWPORT_Y / 2, FOCAL_LENGTH);
Vector3* s;
Ray* cast;

//Returns an intercept where ray intersects the nearest object in the scene
Solid::Intercept* sceneHit(Ray* r) {
	std::list<Solid*>::const_iterator objIterator;
	Solid::Intercept* result = new Solid::Intercept();
	result->mat.color[0] = 25;
	result->mat.color[1] = 25;
	result->mat.color[2] = 25;
	result->normal = new Vector3();
	result->point = new Vector3();
	result->t = std::numeric_limits<float>::infinity();

	for (objIterator = objectList->begin(); objIterator != objectList->end(); objIterator++)
	{
		Solid::Intercept* hit = &((*objIterator)->FindRayIntersect(*r)); //ewwww
		if (hit && hit->t < result->t) {
			result->mat = hit->mat;
			result->normal = hit->normal;
			result->point = hit->point;
			result->t = hit->t;
		}
	}

	return result;
}

//Determines the color of the passed in ray
Solid::Intercept* raycast(Ray* r, int depth)
{
	//Returns background color if maximum recursion depth was hit
	if (depth == 0) { 
		Solid::Intercept* result = new Solid::Intercept();
		result->mat.color[0] = 25;
		result->mat.color[1] = 25;
		result->mat.color[2] = 25;
		result->normal = new Vector3();
		result->point = new Vector3();
		result->t = std::numeric_limits<float>::infinity();
		return result;
	}

	//Find object intersection
	Solid::Intercept* result = sceneHit(r);
	if (result->t != std::numeric_limits<float>::infinity()) {
		//Ambient lighting
		for (int i = 0; i < 3; i++) {
			result->mat.color[i] = result->mat.color[i] * result->mat.shininess;
		}

		//Direct Lighting
		Ray* lightRay = new Ray();
		lightRay->start = result->point;
		lightRay->direction = light->subtract(result->point);
		if (sceneHit(lightRay)->t == std::numeric_limits<float>::infinity()) {
			Vector3* h = (Vector3::normalize(lightRay->direction))->add(Vector3::normalize(r->direction));
			h->normalize();
			for (int i = 0; i < 3; i++) {
				//Diffuse Lighting
				float dot = (lightRay->direction)->dot(result->normal) / (lightRay->direction->mag() * result->normal->mag());
				result->mat.color[i] = result->mat.color[i] + lightColor[i] * result->mat.shininess * max(0, dot);
				//Specular Lighting
				result->mat.color[i] = result->mat.color[i] + lightColor[i] * result->mat.specCoeff * pow(h->dot(result->normal), result->mat.specExponent);
			}
		}

		//Reflected Light
		Ray* reflectRay = new Ray();;
		reflectRay->start = result->point;
		reflectRay->direction = (r->direction->multiply(-1))->add(result->normal->multiply(2*(r->direction->dot(result->normal)))) ; //ewwwwwwwwwwwwww
		Solid::Intercept* reflectHit = raycast(reflectRay, depth - 1);
		for (int i = 0; i < 3; i++) {
			result->mat.color[i] = result->mat.color[i] + result->mat.specCoeff * reflectHit->mat.color[i];
		}
	}

	return result;
}

void renderScene()
{
	//Iterate through all screen pixels
	for (int i = 0; i < screenWidth; i++) {
		for (int j = 0; j < screenHeight; j++) {

			//Set background color
			pixelData[j][i][0] = 0.1;
			pixelData[j][i][1] = 0.1;
			pixelData[j][i][2] = 0.1;

			//Construct viewing ray
			s = new Vector3(
				blCorner->x + (urCorner->x - blCorner->x)*((i + 0.5) / screenWidth),
				blCorner->y + (urCorner->y - blCorner->y)*((j + 0.5) / screenHeight),
				urCorner->z
				);

			cast = new Ray();
			*(cast->start) = *(camera->start);
			*(cast->direction) = *(s->subtract(camera->start));
			
			Solid::Intercept* result = raycast(cast, MAX_RECURSION_DEPTH);

			//If an object was hit, set the color for that pixel appropriately
			for (int k = 0; k < 3; k++) {
				pixelData[j][i][k] = result->mat.color[k];
			}
		}
	}

	glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
}

void constructScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // clear the screen

	light = new Vector3(0.0, 1.0, -0.5);
	lightColor[0] = 178;
	lightColor[1] = 178;
	lightColor[2] = 178;
	ambientLight[0] = 25;
	ambientLight[1] = 25;
	ambientLight[2] = 25;

	objectList = new std::list<Solid*>();

	Box* floor = new Box();
	floor->scale = new Vector3(1.5, 0.05, 1.5);
	floor->position = new Vector3(0.0, -0.5, 0.0);
	floor->mat.color[0] = 150;
	floor->mat.color[1] = 0;
	floor->mat.color[2] = 0;
	floor->mat.shininess = 0.5;
	floor->mat.specCoeff = 0.01;
	floor->mat.specExponent = 10.0;
	floor->mat.reflectionCoeff = 0.05;
	objectList->push_back(floor);

	Box* wall1 = new Box();
	wall1->scale = new Vector3(0.05, 1.025, 1.5);
	wall1->position = new Vector3(-0.775, 0.0, 0.0);
	wall1->mat.color[0] = 178;
	wall1->mat.color[1] = 178;
	wall1->mat.color[2] = 178;
	wall1->mat.shininess = 0.4;
	wall1->mat.specCoeff = 0.1;
	wall1->mat.specExponent = 10.0;
	floor->mat.reflectionCoeff = 0.1;
	objectList->push_back(wall1);

	Box* wall2 = new Box();
	wall2->scale = new Vector3(1.5, 1.025, 0.05);
	wall2->position = new Vector3(0.0, 0.0, -0.775);
	wall2->mat.color[0] = 178;
	wall2->mat.color[1] = 178;
	wall2->mat.color[2] = 178;
	wall2->mat.shininess = 0.4;
	wall2->mat.specCoeff = 0.1;
	wall2->mat.specExponent = 10.0;
	floor->mat.reflectionCoeff = 0.1;
	objectList->push_back(wall2);

	Box* dresser = new Box();
	dresser->scale = new Vector3(0.2f, 0.5f, 0.4f);
	dresser->position = new Vector3(-0.625f, -0.25f, 0.475f);
	dresser->mat.color[0] = 128;
	dresser->mat.color[1] = 102;
	dresser->mat.color[2] = 25;
	dresser->mat.shininess = 0.6;
	dresser->mat.specCoeff = 0.3;
	dresser->mat.specExponent = 400.0;
	floor->mat.reflectionCoeff = 0.3;
	objectList->push_back(dresser);
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
