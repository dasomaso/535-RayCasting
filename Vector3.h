#include <stdlib.h>
#include <vector>
#include <math.h>
#include <array>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>

#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3
{
public:
	GLfloat x, y, z;
	Vector3() { x = 0.0f; y = 0.0f; z = 0.0f; }
	Vector3(GLfloat _x, GLfloat _y, GLfloat _z) { x = _x; y = _y; z = _z; }
	float mag() { return sqrtf(x*x + y*y + z*z); }
	void normalize() { x = x / mag(); y = y / mag(); z = z / mag(); }
	
	void print() { std::cout << "(" << x << ", " << y << ", " << z << ")"; }

	Vector3* average(Vector3* v2) 
	{ 
		return new Vector3(
			(x + v2->x) / 2,
			(y + v2->y) / 2,
			(z + v2->z) / 2);
	}

	Vector3* add(Vector3* v2)
	{
		return new Vector3(
			(x + v2->x),
			(y + v2->y),
			(z + v2->z));
	}

	Vector3* subtract(Vector3* v2)
	{
		return new Vector3(
			(x - v2->x),
			(y - v2->y),
			(z - v2->z));
	}

	Vector3* divide(float c)
	{
		return new Vector3(
			(x / c),
			(y / c),
			(z / c));
	}
};
#endif