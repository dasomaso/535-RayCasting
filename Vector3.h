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
	float x, y, z;
	Vector3() { x = 0.0f; y = 0.0f; z = 0.0f; }
	Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	float mag() { return sqrtf(x*x + y*y + z*z); }
	void normalize() { x = x / mag(); y = y / mag(); z = z / mag(); }
	
	void print() { std::cout << "(" << x << ", " << y << ", " << z << ")"; }

	Vector3 average(Vector3* v2) 
	{ 
		Vector3 ans;
		ans.x = (x + v2->x) / 2;
		ans.y = (y + v2->y) / 2;
		ans.z = (z + v2->z) / 2;
		return ans;
	}

	Vector3 add(Vector3* v2)
	{
		Vector3 ans;
		ans.x = (x + v2->x);
		ans.y = (y + v2->y);
		ans.z = (z + v2->z);
		return ans;
	}

	Vector3 subtract(Vector3* v2)
	{
		Vector3 ans;
		ans.x = (x - v2->x);
		ans.y = (y - v2->y);
		ans.z = (z - v2->z);
		return ans;
	}

	Vector3 multiply(Vector3* v2)
	{
		Vector3 ans;
		ans.x = (x * v2->x);
		ans.y = (y * v2->y);
		ans.z = (z * v2->z);
		return ans;
	}

	Vector3 multiply(float c)
	{
		Vector3 ans;
		ans.x = (x * c);
		ans.y = (y * c);
		ans.z = (z * c);
		return ans;
	}

	Vector3 divide(Vector3* v2)
	{
		Vector3 ans;
		ans.x = (x / v2->x);
		ans.y = (y / v2->y);
		ans.z = (z / v2->z);
		return ans;
	}

	Vector3 divide(float c)
	{
		Vector3 ans;
		ans.x = (x / c);
		ans.y = (y / c);
		ans.z = (z / c);
		return ans;
	}

	float dot(Vector3* v2)
	{
		return x*v2->x + y*v2->y + z*v2->z;
	}

	void transform(Vector3* pos, Vector3* rot, Vector3* scale) {
		x = x*scale->x;
		y = y*scale->y;
		z = z*scale->z;

		x = x+pos->x;
		y = y+pos->y;
		z = z+pos->z;
	}

	Vector3 refract(Vector3* n, float index) {
		Vector3 ans;
		float dn = dot(n);
		float root = 1 - (1 - dn * dn) / (index * index);
		if (root >= 0) {
			ans = subtract(&(n->multiply(dn)));
			ans = ans.divide(index);
			ans = ans.subtract(&(n->multiply(root)));
		}

		return ans;
	}
};
#endif