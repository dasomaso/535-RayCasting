#include <stdlib.h>
#include <vector>
#include <math.h>
#include <array>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>
#include <limits>
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include "Solid.h"

#ifndef BOX_H
#define BOX_H

class Box : public Solid
{
public:
	Vector3* position;
	Vector3* rotation;
	Vector3* scale;
	Material mat;

	Vector3* corners[2];
	Vector3* normals[6];

	Box() {
		position = new Vector3();
		rotation = new Vector3();
		scale = new Vector3(1.0, 1.0, 1.0);
		mat.color[0] = 1.0;
		mat.color[1] = 1.0;
		mat.color[2] = 1.0;
		mat.alpha = 1.0;
		mat.shininess = 1.0;

		corners[0] = new Vector3(0.5, 0.5, 0.5);
		corners[1] = new Vector3(-0.5, -0.5, -0.5);

		normals[0] = new Vector3(1.0, 0.0, 0.0);
		normals[1] = new Vector3(0.0, 1.0, 0.0);
		normals[2] = new Vector3(0.0, 0.0, 1.0);
		normals[3] = new Vector3(-1.0, 0.0, 0.0);
		normals[4] = new Vector3(0.0, -1.0, 0.0);
		normals[5] = new Vector3(0.0, 0.0, -1.0);
	}

	Vector3* BoundingBox() {
		Vector3* box[2];
		box[0] = position->subtract(scale->divide(2));
		box[1] = position->add(scale->divide(2));
		return *box;
	}

	Intercept* FindRayIntersect(Ray* ray);
};

#endif