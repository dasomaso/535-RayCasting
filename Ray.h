#include <stdlib.h>
#include <vector>
#include <math.h>
#include <array>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>

#include "Vector3.h"

#ifndef RAY_H
#define RAY_H

class Ray
{
public:
	Vector3* start;
	Vector3* direction;

	Ray() { start = new Vector3(); direction = new Vector3(); }
	Ray(Vector3* s, Vector3* d) { start = s; direction = d; }

	//Reverses given translation, rotation, and scale transformations
	void inverseTransform(Vector3* pos, Vector3* rot, Vector3* scale) {
		*start = start->subtract(pos);
		*start = start->divide(scale);

		*direction = direction->divide(scale);
	}

	//Applies translation, rotation, and scale transformations
	void transform(Vector3* pos, Vector3* rot, Vector3* scale) {
		*start = start->multiply(scale);
		*start = start->add(pos);

		*direction = direction->multiply(scale);
	}

	void print() {
		std::cout << "(" << start->x << "," << start->y << "," << start->z << ") -> (" << direction->x << "," << direction->y << "," << direction->z << ")";
	}
};
#endif