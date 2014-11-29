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

	void transform(Vector3* pos, Vector3* rot, Vector3* scale) {
		Vector3* base = start;

		start->subtract(base);
		direction->subtract(base);

		direction->divide(scale);

		start->add(base);
		start->subtract(pos);
		direction->add(base);
		direction->subtract(pos);
	}
};
#endif