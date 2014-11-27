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

#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>
#include <time.h>

#include "Solid.h"

const double PI = 3.1415926;
const int screenWidth = 640;
const int screenHeight = 480;

clock_t startTime;
const double fanSpeed = 540;	//Speed of fan rotation is degree/second

void myDisplay(void)
{
	// set properties of the surface material
	GLfloat mat_ambient_grey[] = { 0.7f, 0.7f, 0.7f, 1.0f };   //gray
	GLfloat mat_diffuse_grey[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat mat_specular_grey[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat mat_ambient_red [] = {0.7f, 0.0f, 0.0f, 1.0f};   //red
	GLfloat mat_diffuse_red [] = {0.6f, 0.0f, 0.0f, 1.0f};
	GLfloat mat_specular_red [] = {1.0f, 0.0f, 0.0f, 1.0f};

	GLfloat mat_ambient_blue[] = { 0.0f, 0.0f, 0.8f, 1.0f };   //blue
	GLfloat mat_diffuse_blue[] = { 0.0f, 0.0f, 0.7f, 1.0f };
	GLfloat mat_specular_blue[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	GLfloat mat_ambient_brown[] = { 0.5f, 0.4f, 0.1f, 1.0f };   //brown
	GLfloat mat_diffuse_brown[] = { 0.4f, 0.3f, 0.0f, 1.0f };
	GLfloat mat_specular_brown[] = { 1.0f, 0.9f, 0.5f, 1.0f };

	GLfloat mat_ambient_white[] = { 0.95f, 0.95f, 0.95f, 1.0f };   //white
	GLfloat mat_diffuse_white[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat mat_specular_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat mat_ambient_gold[] = { 0.9f, 0.8f, 0.0f, 1.0f };   //gold
	GLfloat mat_diffuse_gold[] = { 0.8f, 0.7f, 0.0f, 1.0f };
	GLfloat mat_specular_gold[] = { 1.0f, 1.0f, 0.7f, 1.0f };

	GLfloat mat_ambient_black[] = { 0.2f, 0.2f, 0.2f, 1.0f };   //black
	GLfloat mat_diffuse_black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_specular_black[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	GLfloat mat_shininess[] = { 50.0f };
	
	// set the light source properties
	GLfloat lightIntensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat light_position[] = { 2.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

	glMatrixMode(GL_MODELVIEW);      // position and aim the camera
	glLoadIdentity();
	gluLookAt(2.3, 1.3, 2.0, 0.0, 0.25, 0.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, screenWidth / screenHeight, 0.2f, 255.0f);

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // clear the screen
	
	//Realized that I had made a sort of solid pseudo-factory, so I only need 1 solid
	//Could have made a nicer solid that hid more openGL calls if I included material and transformation in the class
	Solid* solidGen = new Solid();

	//Shift for aesthetics
	glTranslatef(0.0f, 0.2f, 0.0f);

	//----------Boxes----------//
	solidGen->GenerateBox();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_red);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_red);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//Floor
	glPushMatrix();
		glTranslatef(0.0f, -0.5f, 0.0f);
		glScalef(1.5f, 0.05f, 1.5f);
		solidGen->render();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_grey);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_grey);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_grey);

	//Walls
	glPushMatrix();
		glTranslatef(-0.75f, 0.0f, 0.0f);
		glScalef(0.05f, 1.0f, 1.5f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -0.75f);
		glScalef(1.5f, 1.0f, 0.05f);
		solidGen->render();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_brown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_brown);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_brown);

	//Bed Frame
	glPushMatrix();
		glTranslatef(-0.525f, -0.4f, -0.325f);
		glScalef(0.4f, 0.1f, 0.8f);
		solidGen->render();
	glPopMatrix();

	//Nightstand
	glPushMatrix();
		glTranslatef(0.0f, -0.25f, -0.6f);
		glScalef(0.25f, 0.02f, 0.25f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.11f, -0.375f, -0.6f);
		glScalef(0.02f, 0.25f, 0.2f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.11f, -0.375f, -0.6f);
		glScalef(0.02f, 0.25f, 0.2f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, -0.375f, -0.71f);
		glScalef(0.2f, 0.25f, 0.02f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, -0.375f, -0.6f);
		glScalef(0.21f, 0.02f, 0.21f);
		solidGen->render();
	glPopMatrix();

	//Dresser
	glPushMatrix();
		glTranslatef(-0.625f, -0.25f, 0.475f);
		glScalef(0.2f, 0.5f, 0.4f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.625f, -0.3f, 0.475f);
		glScalef(0.22f, 0.09f, 0.35f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.625f, -0.4f, 0.475f);
		glScalef(0.22f, 0.09f, 0.35f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.625f, -0.2f, 0.475f);
		glScalef(0.22f, 0.09f, 0.35f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.625f, -0.1f, 0.475f);
		glScalef(0.22f, 0.09f, 0.35f);
		solidGen->render();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_white);

	//Mattress
	glPushMatrix();
		glTranslatef(-0.525f, -0.375f, -0.325f);
		glScalef(0.375f, 0.1f, 0.775f);
		solidGen->render();
	glPopMatrix();

	//----------Cylinders----------//
	solidGen->GenerateCylinder(40);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_brown);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_brown);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_brown);

	//Bed Legs
	glPushMatrix();
		glTranslatef(-0.3375f, -0.475f, 0.0625f);
		glScalef(0.025f, 0.1f, 0.025f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.7125f, -0.475f, -0.7125f);
		glScalef(0.025f, 0.1f, 0.05f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.3375f, -0.475f, -0.7125f);
		glScalef(0.025f, 0.1f, 0.025f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.7125f, -0.475f, 0.0625f);
		glScalef(0.025f, 0.1f, 0.025f);
		solidGen->render();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_white);

	//Pillow
	glPushMatrix();
		glTranslatef(-0.525f, -0.3f, -0.66f);
		glScalef(0.3f, 0.05f, 0.15f);
		glRotatef(90, 0, 0, 1);
		solidGen->render();
	glPopMatrix();

	//Fan Base
	glPushMatrix();
		glTranslatef(0.6f, -0.435f, -0.3f);
		glScalef(0.15f, 0.015f, 0.15f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.6f, -0.235f, -0.3f);
		glScalef(0.015f, 0.45f, 0.015f);
		solidGen->render();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_gold);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_gold);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_gold);

	//Lamp
	glPushMatrix();
		glTranslatef(0.05f, -0.23f, -0.65f);
		glScalef(0.1f, 0.02f, 0.1f);
		solidGen->render();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.05f, -0.17f, -0.65f);
		glScalef(0.02f, 0.1f, 0.02f);
		solidGen->render();
	glPopMatrix();

	//----------Cones----------//
	solidGen->GenerateCone(30);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_white);

	glPushMatrix();
		glTranslatef(0.05f, -0.1f, -0.65f);
		glScalef(0.1f, 0.15f, 0.1f);
		solidGen->render();
	glPopMatrix();

	//----------Spheres----------//
	solidGen->GenerateSphere(30);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_blue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_blue);

	glPushMatrix();
		glTranslatef(-0.575f, 0.05f, 0.6f);
		glScalef(0.1f, 0.1f, 0.1f);
		solidGen->render();
	glPopMatrix();


	//--------Fan Head--------//
	solidGen->GenerateCylinder(30);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_white);

	glPushMatrix();
		glTranslatef(0.63f, -0.03f, -0.3f);
		glRotatef(90, 0, 0, 1);
		glRotatef(180, 0, 1, 0);
		glScalef(0.05f, 0.07f, 0.05f);
		solidGen->render();
	glPopMatrix();

	solidGen->GenerateSphere(15);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_black);

	glPushMatrix();
		glTranslatef(0.595f, -0.03f, -0.3f);
		glScalef(0.05f, 0.05f, 0.05f);
		solidGen->render();
	glPopMatrix();

	solidGen->GenerateCylinder(30);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_white);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_white);

	float bladeRot = fmod(fanSpeed * (((float)(clock() - startTime))/CLOCKS_PER_SEC), 360);
	glPushMatrix();
		glTranslatef(0.5875f, -0.03f, -0.3f);
		glRotatef(90, 0, 0, 1);
		glRotatef(bladeRot, 0, 1, 0);

		glPushMatrix();
			glTranslatef(0.0525f, 0.0f, 0.0f);
			glScalef(0.15f, 0.001f, 0.05f);
			solidGen->render();
		glPopMatrix();

		glPushMatrix();
			glRotatef(120, 0, 1, 0);
			glTranslatef(0.0525f, 0.0f, 0.0f);
			glScalef(0.15f, 0.001f, 0.05f);
			solidGen->render();
		glPopMatrix();

		glPushMatrix();
			glRotatef(240, 0, 1, 0);
			glTranslatef(0.0525f, 0.0f, 0.0f);
			glScalef(0.15f, 0.001f, 0.05f);
			solidGen->render();
		glPopMatrix();

	glPopMatrix();

	
	glFlush();

	glutPostRedisplay();
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
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
	startTime = clock(); // Set up frame timer
	glutMainLoop();
}
