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
};
#endif