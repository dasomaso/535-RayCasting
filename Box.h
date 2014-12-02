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
		mat.alpha = 1.0;
		mat.refractIndex = 1.0;
		mat.specExponent = 200.0;
		mat.reflectionCoeff = 0.0;
	}

	Intercept FindRayIntersect(Ray ray);
};

#endif