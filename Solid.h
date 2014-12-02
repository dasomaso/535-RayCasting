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

#ifndef SOLID_H
#define SOLID_H

class Solid
{
public:
	struct Material {
		float color[3]; //rgb color
		float ambientColor[3];
		float specularColor[3];
		float diffuseColor[3];
		float alpha;
		float refractIndex;
		float specExponent;
		float reflectionCoeff;
	};

	struct Intercept {
		Solid::Material mat;
		Vector3* point;
		Vector3* normal;
		float t;
	};

	Vector3* position; 
	Vector3* rotation;
	Vector3* scale;
	Solid::Material mat;


	Solid() {
		position = new Vector3();
		rotation = new Vector3();
		scale = new Vector3(1.0, 1.0, 1.0);
		mat.alpha = 1.0;
		mat.refractIndex = 1.0;
		mat.specExponent = 200.0;
		mat.reflectionCoeff = 0.0;
	}

	virtual Solid::Intercept FindRayIntersect(Ray ray) = 0;
};

#endif