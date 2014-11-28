#include <stdlib.h>
#include <vector>
#include <math.h>
#include <array>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <Windows.h>

#include "Vector3.h"

#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
public:
	//all numbers are 0 to 1
	float color[3]; //rgb color
	float alpha;
	float shininess; //coefficient of reflection

	Material() {}
};
#endif