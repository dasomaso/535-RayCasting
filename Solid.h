#include <stdlib.h>
#include <vector>
#include <math.h>
#include <array>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>
#include "Vector3.h"

#ifndef SOLID_H
#define SOLID_H

class Solid
{
public:
	Vector3* position; 
	Vector3* rotation;
	Vector3* scale;

	Solid() {
		position = new Vector3();
		rotation = new Vector3();
		scale = new Vector3(1.0, 1.0, 1.0);
	}

	virtual Vector3 FindRayIntersect(Vector3 ray) = 0;
};

#endif