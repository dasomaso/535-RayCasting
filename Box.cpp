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
#include "Solid.h"
#include "Box.h"

Solid::Intercept* Box::FindRayIntersect(Ray* ray) {
	std::cout << "Finding intercept\n";

	Solid::Intercept* result;
	result->mat = mat;
	result->normal = normals[0];
	result->t = std::numeric_limits<float>::infinity();

	ray->transform(position, rotation, scale);

	float t = std::numeric_limits<float>::infinity();
	//+x face
	float tmp_t = ((corners[0]->subtract(ray->start))->dot(normals[0])) / (ray->direction->dot(normals[0]));
	Vector3* intersect = ray->start->add(ray->direction->multiply(tmp_t));
	if (intersect->y >= -0.5 && intersect->y <= 0.5
		&& intersect->z >= -0.5 && intersect->z <= 0.5
		&& tmp_t < result->t) {
		delete result;
		result = new Solid::Intercept();
		result->mat = mat;
		result->normal = normals[0];
		result->t = tmp_t;
	}

	//+y face
	tmp_t = ((corners[0]->subtract(ray->start))->dot(normals[1])) / (ray->direction->dot(normals[1]));
	intersect = ray->start->add(ray->direction->multiply(tmp_t));
	if (intersect->x >= -0.5 && intersect->x <= 0.5
		&& intersect->z >= -0.5 && intersect->z <= 0.5
		&& tmp_t < result->t) {
		delete result;
		result = new Solid::Intercept();
		result->mat = mat;
		result->normal = normals[1];
		result->t = tmp_t;
	}

	//+z face
	tmp_t = ((corners[0]->subtract(ray->start))->dot(normals[2])) / (ray->direction->dot(normals[2]));
	intersect = ray->start->add(ray->direction->multiply(tmp_t));
	if (intersect->x >= -0.5 && intersect->x <= 0.5
		&& intersect->y >= -0.5 && intersect->y <= 0.5
		&& tmp_t < result->t) {
		delete result;
		result = new Solid::Intercept();
		result->mat = mat;
		result->normal = normals[2];
		result->t = tmp_t;
	}

	//-x face
	tmp_t = ((corners[1]->subtract(ray->start))->dot(normals[3])) / (ray->direction->dot(normals[3]));
	intersect = ray->start->add(ray->direction->multiply(tmp_t));
	if (intersect->z >= -0.5 && intersect->z <= 0.5
		&& intersect->y >= -0.5 && intersect->y <= 0.5
		&& tmp_t < result->t) {
		delete result;
		result = new Solid::Intercept();
		result->mat = mat;
		result->normal = normals[3];
		result->t = tmp_t;
	}

	//-y face
	tmp_t = ((corners[1]->subtract(ray->start))->dot(normals[4])) / (ray->direction->dot(normals[4]));
	intersect = ray->start->add(ray->direction->multiply(tmp_t));
	if (intersect->z >= -0.5 && intersect->z <= 0.5
		&& intersect->x >= -0.5 && intersect->x <= 0.5
		&& tmp_t < result->t) {
		delete result;
		result = new Solid::Intercept();
		result->mat = mat;
		result->normal = normals[4];
		result->t = tmp_t;
	}

	//-z face
	tmp_t = ((corners[1]->subtract(ray->start))->dot(normals[5])) / (ray->direction->dot(normals[5]));
	intersect = ray->start->add(ray->direction->multiply(tmp_t));
	if (intersect->x >= -0.5 && intersect->x <= 0.5
		&& intersect->y >= -0.5 && intersect->y <= 0.5
		&& tmp_t < result->t) {
		delete result;
		result = new Solid::Intercept();
		result->mat = mat;
		result->normal = normals[5];
		result->t = tmp_t;
	}

	if (result->t != std::numeric_limits<float>::infinity()) {
		return result;
	}
	else {
		return NULL;
	}
}