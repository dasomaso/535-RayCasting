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

Solid::Intercept Box::FindRayIntersect(Ray ray) {
	Solid::Intercept result;
	result.mat = mat;
	result.normal = new Vector3();
	result.point = new Vector3();
	result.t = std::numeric_limits<float>::infinity();

	//Transform ray to local coordinates
	ray.inverseTransform(position, rotation, scale);

	float tmp_t = std::numeric_limits<float>::infinity();
	Vector3* intersect;
	//+x face
	tmp_t = (0.5 - ray.start->x) / (ray.direction->x);
	intersect = ray.start->add(ray.direction->multiply(tmp_t));
	if (intersect->y >= -0.5 && intersect->y <= 0.5
		&& intersect->z >= -0.5 && intersect->z <= 0.5
		&& tmp_t < result.t) {
		result.mat = mat;
		result.point = intersect->transform(*position, *rotation, *scale);
		result.normal = new Vector3(1.0, 0.0, 0.0);
		result.t = tmp_t;
	}

	//+y face
	tmp_t = (0.5 - ray.start->y) / (ray.direction->y);
	intersect = ray.start->add(ray.direction->multiply(tmp_t));
	if (intersect->x >= -0.5 && intersect->x <= 0.5
		&& intersect->z >= -0.5 && intersect->z <= 0.5
		&& tmp_t < result.t) {
		result.mat = mat;
		result.point = intersect->transform(*position, *rotation, *scale);
		result.normal = new Vector3(0.0, 1.0, 0.0);
		result.t = tmp_t;
	}

	//+z face
	tmp_t = (0.5 - ray.start->z) / (ray.direction->z);
	intersect = ray.start->add(ray.direction->multiply(tmp_t));
	if (intersect->x >= -0.5 && intersect->x <= 0.5
		&& intersect->y >= -0.5 && intersect->y <= 0.5
		&& tmp_t < result.t) {
		result.mat = mat;
		result.point = intersect->transform(*position, *rotation, *scale);
		result.normal = new Vector3(0.0, 0.0, 1.0);
		result.t = tmp_t;
	}

	//-x face
	tmp_t = (-0.5 - ray.start->x) / (ray.direction->x);
	intersect = ray.start->add(ray.direction->multiply(tmp_t));
	if (intersect->z >= -0.5 && intersect->z <= 0.5
		&& intersect->y >= -0.5 && intersect->y <= 0.5
		&& tmp_t < result.t) {
		result.mat = mat;
		result.point = intersect->transform(*position, *rotation, *scale);
		result.normal = new Vector3(-1.0, 0.0, 0.0);
		result.t = tmp_t;
	}

	//-y face
	tmp_t = (-0.5 - ray.start->y) / (ray.direction->y);
	intersect = ray.start->add(ray.direction->multiply(tmp_t));
	if (intersect->z >= -0.5 && intersect->z <= 0.5
		&& intersect->x >= -0.5 && intersect->x <= 0.5
		&& tmp_t < result.t) {
		result.mat = mat;
		result.point = intersect->transform(*position, *rotation, *scale);
		result.normal = new Vector3(0.0, -1.0, 0.0);
		result.t = tmp_t;
	}

	//-z face
	tmp_t = (-0.5 - ray.start->z) / (ray.direction->z);
	intersect = ray.start->add(ray.direction->multiply(tmp_t));
	if (intersect->x >= -0.5 && intersect->x <= 0.5
		&& intersect->y >= -0.5 && intersect->y <= 0.5
		&& tmp_t < result.t) {
		result.mat = mat;
		result.point = intersect->transform(*position, *rotation, *scale);
		result.normal = new Vector3(0.0, 0.0, -1.0);
		result.t = tmp_t;
	}

	//Transforms normals from local to world coords
	result.normal->transform(*position, *rotation, *scale);
	result.normal->normalize();

	return result;
}