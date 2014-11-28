#include <stdlib.h>
#include <vector>
#include <math.h>
#include <array>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"

#ifndef SOLID_H
#define SOLID_H

class Solid
{
public:
	Vector3* position; 
	Vector3* rotation;
	Vector3* scale;
	Material mat;


	Solid() {
		position = new Vector3();
		rotation = new Vector3();
		scale = new Vector3(1.0, 1.0, 1.0);
		mat.color[0] = 1.0;
		mat.color[1] = 1.0;
		mat.color[2] = 1.0;
		mat.alpha = 1.0;
		mat.shininess = 1.0;
	}

	Vector3* BoundingBox() { 
		Vector3* box[2];
		box[0] = position->subtract(scale->divide(2));
		box[1] = position->add(scale->divide(2));
		return *box;
	}

	virtual Vector3 FindRayIntersect(Ray ray) = 0;
};

#endif