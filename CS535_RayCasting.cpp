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
#include "Sphere.h"
#include "Vector3.h"
#include "Ray.h"

const int MAX_RECURSION_DEPTH = 8;
const float VIEWPORT_X = 3;
const float VIEWPORT_Y = 2.25;
const float FOCAL_LENGTH = -0.5;
const int screenWidth = 800;
const int screenHeight = 600;

GLubyte pixelData[screenHeight][screenWidth][3];

Vector3* light;
float lightColor[3];
float ambientLight[3];

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
		Solid::Intercept hit = (*objIterator)->FindRayIntersect(*r);
		if (hit.t < result->t) {
			result->mat = hit.mat;
			result->normal = hit.normal;
			result->point = hit.point;
			result->t = hit.t;
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
		result->mat.color[0] = 0.1;
		result->mat.color[1] = 0.1;
		result->mat.color[2] = 0.1;
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
			result->mat.color[i] = result->mat.ambientColor[i] * ambientLight[i];
		}

		//Direct Lighting
		Ray* lightRay = new Ray();
		*(lightRay->start) = *(result->point);
		*(lightRay->direction) = light->subtract(result->point);
		if (sceneHit(lightRay)->t == std::numeric_limits<float>::infinity()) {
			lightRay->direction->normalize();
			r->direction->normalize();
			Vector3 h = (lightRay->direction)->subtract(r->direction);
			h.normalize();
			for (int i = 0; i < 3; i++) {
				//Diffuse Lighting
				float dot = (lightRay->direction)->dot(result->normal)/* / (lightRay->direction->mag() * result->normal->mag())*/;
				result->mat.color[i] = result->mat.color[i] + lightColor[i] * result->mat.diffuseColor[i] * max(0, dot);
				//Specular Lighting
				float hnorm = pow(h.dot(result->normal), result->mat.specExponent);
				result->mat.color[i] = result->mat.color[i] + lightColor[i] * result->mat.specularColor[i] * hnorm;
			}
		}

		//Reflected Light
		if (result->mat.specularColor[0] > 0 || result->mat.specularColor[1] > 0 || result->mat.specularColor[0] > 2) {
			Ray* reflectRay = new Ray();
			*(reflectRay->start) = *(result->point);
			*(reflectRay->direction) = *(r->direction);
			float dn = r->direction->dot(result->normal);
			*(reflectRay->direction) = reflectRay->direction->subtract(&(result->normal->multiply(2 * dn))); //ewwwwwwwwwwwwww
			Solid::Intercept* reflectHit = raycast(reflectRay, depth - 1);
			for (int i = 0; i < 3; i++) {
				result->mat.color[i] = result->mat.color[i] + result->mat.specularColor[i] * reflectHit->mat.color[i];
			}

			delete reflectRay;
			delete reflectHit;
		}

		//Refraction
		if (result->mat.alpha > 0) {
			Ray* refractedRay = new Ray();

			//Find point refracted through object
			*(refractedRay->start) = *(result->point);
			*(refractedRay->direction) = r->direction->refract(result->normal, 1.0, result->mat.refractIndex);
			refractedRay->direction->normalize();
			Solid::Intercept* refractHit = sceneHit(refractedRay);

			//Find exit ray from object
			*(refractedRay->start) = *(refractHit->point);
			*(refractedRay->direction) = refractedRay->direction->refract(&(refractHit->normal->multiply(-1)), result->mat.refractIndex, 1.0);
			refractedRay->direction->normalize();

			if (refractedRay->direction->x != 0 && refractedRay->direction->y != 0 && refractedRay->direction->z != 0) {
				refractHit = raycast(refractedRay, depth - 1);

				for (int i = 0; i < 3; i++) {
					result->mat.color[i] = (1 - result->mat.alpha) * result->mat.color[i] + result->mat.alpha * refractHit->mat.color[i];
				}
			}

			delete refractedRay;
			delete refractHit;
		}

		delete lightRay;
		

		return result;
	}
	else {
		result->mat.color[0] = 0.1;
		result->mat.color[1] = 0.1;
		result->mat.color[2] = 0.1;

		return result;
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
				blCorner->x + (urCorner->x - blCorner->x)*((i + 0.5) / screenWidth),
				blCorner->y + (urCorner->y - blCorner->y)*((j + 0.5) / screenHeight),
				urCorner->z
				);

			cast = new Ray();
			*(cast->start) = *(camera->start);
			*(cast->direction) = s->subtract(camera->start);
			
			Solid::Intercept* result = raycast(cast, MAX_RECURSION_DEPTH);

			//If an object was hit, set the color for that pixel appropriately
			for (int k = 0; k < 3; k++) {
				pixelData[j][i][k] = min(result->mat.color[k] * 255, 255);
			}
		}
	}

	glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
}

void constructScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // clear the screen

	light = new Vector3(0.6, 3.0, -0.25);
	lightColor[0] = 1.0;
	lightColor[1] = 1.0;
	lightColor[2] = 1.0;
	ambientLight[0] = 0.5;
	ambientLight[1] = 0.5;
	ambientLight[2] = 0.5;

	objectList = new std::list<Solid*>();

	Box* floor = new Box();
	floor->scale = new Vector3(1.5, 0.05, 1.5);
	floor->position = new Vector3(0.0, -0.5, 0.0);
	floor->mat.diffuseColor[0] = 0.5;
	floor->mat.diffuseColor[1] = 0;
	floor->mat.diffuseColor[2] = 0;
	floor->mat.ambientColor[0] = 0.6;
	floor->mat.ambientColor[1] = 0;
	floor->mat.ambientColor[2] = 0;
	floor->mat.specularColor[0] = 0;
	floor->mat.specularColor[1] = 0;
	floor->mat.specularColor[2] = 0;
	floor->mat.alpha = 0;
	floor->mat.specExponent = 10.0;
	objectList->push_back(floor);

	Box* wall1 = new Box();
	wall1->scale = new Vector3(0.05, 1.025, 1.5);
	wall1->position = new Vector3(-0.775, 0.0, 0.0);
	wall1->mat.diffuseColor[0] = 0.0;
	wall1->mat.diffuseColor[1] = 0.0;
	wall1->mat.diffuseColor[2] = 0.0;
	wall1->mat.ambientColor[0] = 0.0;
	wall1->mat.ambientColor[1] = 0.0;
	wall1->mat.ambientColor[2] = 0.0;
	wall1->mat.specularColor[0] = 1.0;
	wall1->mat.specularColor[1] = 1.0;
	wall1->mat.specularColor[2] = 1.0;
	wall1->mat.alpha = 0;
	wall1->mat.specExponent = 10.0;
	objectList->push_back(wall1);

	Box* wall2 = new Box();
	wall2->scale = new Vector3(1.5, 1.025, 0.05);
	wall2->position = new Vector3(0.0, 0.0, -0.775);
	wall2->mat.diffuseColor[0] = 0.0;
	wall2->mat.diffuseColor[1] = 0.0;
	wall2->mat.diffuseColor[2] = 0.0;
	wall2->mat.ambientColor[0] = 0.0;
	wall2->mat.ambientColor[1] = 0.0;
	wall2->mat.ambientColor[2] = 0.0;
	wall2->mat.specularColor[0] = 1.0;
	wall2->mat.specularColor[1] = 1.0;
	wall2->mat.specularColor[2] = 1.0;
	wall2->mat.alpha = 0;
	wall2->mat.specExponent = 10.0;
	objectList->push_back(wall2);

	Box* wall3 = new Box();
	wall3->scale = new Vector3(0.05, 1.025, 1.5);
	wall3->position = new Vector3(0.775, 0.0, 0.0);
	wall3->mat.diffuseColor[0] = 0.6;
	wall3->mat.diffuseColor[1] = 0.6;
	wall3->mat.diffuseColor[2] = 0.6;
	wall3->mat.ambientColor[0] = 0.7;
	wall3->mat.ambientColor[1] = 0.7;
	wall3->mat.ambientColor[2] = 0.7;
	wall3->mat.specularColor[0] = 0.0;
	wall3->mat.specularColor[1] = 0;
	wall3->mat.specularColor[2] = 0;
	wall3->mat.alpha = 0;
	wall3->mat.specExponent = 10.0;
	objectList->push_back(wall3);

	Box* wall4 = new Box();
	wall4->scale = new Vector3(0.75, 1.025, 0.05);
	wall4->position = new Vector3(0.3875, 0.0, 0.775);
	wall4->mat.diffuseColor[0] = 0.6;
	wall4->mat.diffuseColor[1] = 0.0;
	wall4->mat.diffuseColor[2] = 0.6;
	wall4->mat.ambientColor[0] = 0.6;
	wall4->mat.ambientColor[1] = 0.0;
	wall4->mat.ambientColor[2] = 0.6;
	wall4->mat.specularColor[0] = 0.0;
	wall4->mat.specularColor[1] = 0.0;
	wall4->mat.specularColor[2] = 0.0;
	wall4->mat.alpha = 0.6;
	wall4->mat.refractIndex = 1.5;
	wall4->mat.specExponent = 10.0;
	objectList->push_back(wall4);

	Box* dresser = new Box();
	dresser->scale = new Vector3(0.2f, 0.5f, 0.4f);
	dresser->position = new Vector3(-0.625f, -0.25f, 0.475f);
	dresser->mat.diffuseColor[0] = 0.4;
	dresser->mat.diffuseColor[1] = 0.3;
	dresser->mat.diffuseColor[2] = 0;
	dresser->mat.ambientColor[0] = 0.5;
	dresser->mat.ambientColor[1] = 0.4;
	dresser->mat.ambientColor[2] = 0.1;
	dresser->mat.specularColor[0] = 0.3;
	dresser->mat.specularColor[1] = 0.1;
	dresser->mat.specularColor[2] = 0.0;
	dresser->mat.alpha = 0;
	dresser->mat.specExponent = 200.0;
	objectList->push_back(dresser);

	Sphere* ball = new Sphere();
	ball->scale = new Vector3(0.2, 0.2, 0.2);
	ball->position = new Vector3(0, -0.25, 0);
	ball->mat.diffuseColor[0] = 0;
	ball->mat.diffuseColor[1] = 0;
	ball->mat.diffuseColor[2] = 0.6;
	ball->mat.ambientColor[0] = 0;
	ball->mat.ambientColor[1] = 0;
	ball->mat.ambientColor[2] = 0.7;
	ball->mat.specularColor[0] = 0.2;
	ball->mat.specularColor[1] = 0.2;
	ball->mat.specularColor[2] = 0.8;
	ball->mat.alpha = 0.3;
	ball->mat.refractIndex = 1.5;
	ball->mat.specExponent = 200.0;
	objectList->push_back(ball);

	Box* cube = new Box();
	cube->scale = new Vector3(0.15f, 0.15f, 0.15f);
	cube->position = new Vector3(-0.35f, -0.3f, 0.4f);
	cube->mat.diffuseColor[0] = 0.0;
	cube->mat.diffuseColor[1] = 1.0;
	cube->mat.diffuseColor[2] = 0;
	cube->mat.ambientColor[0] = 0;
	cube->mat.ambientColor[1] = 0.7;
	cube->mat.ambientColor[2] = 0;
	cube->mat.specularColor[0] = 0;
	cube->mat.specularColor[1] = 0;
	cube->mat.specularColor[2] = 0;
	cube->mat.alpha = 0;
	cube->mat.specExponent = 20.0;
	objectList->push_back(cube);
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
