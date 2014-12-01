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

#ifndef SPHERE_H
#define SPHERE_H

class Sphere : public Solid
{
public:
	Vector3* position;
	Vector3* rotation;
	Vector3* scale;
	Solid::Material mat;

	Sphere() {
		position = new Vector3();
		rotation = new Vector3();
		scale = new Vector3(1.0, 1.0, 1.0);
		mat.color[0] = 255;
		mat.color[1] = 255;
		mat.color[2] = 255;
		mat.alpha = 1.0;
		mat.refractIndex = 1.0;
		mat.shininess = 1.0;
		mat.specCoeff = 1.0;
		mat.specExponent = 200.0;
		mat.reflectionCoeff = 0.0;
	}

	Intercept FindRayIntersect(Ray ray);
};

#endif