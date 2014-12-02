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
#include "Sphere.h"

Solid::Intercept Sphere::FindRayIntersect(Ray ray) {
	Solid::Intercept result;
	result.mat = mat;
	result.normal = new Vector3();
	result.point = new Vector3();
	result.t = std::numeric_limits<float>::infinity();

	//Transform ray to local coordinates
	ray.inverseTransform(position, rotation, scale);
	
	float quadratic = 4 * (ray.direction->dot(ray.start)) * (ray.direction->dot(ray.start))
		- 4 * ray.direction->mag() * ray.direction->mag() * (ray.start->mag() * ray.start->mag() - 1);
	if (quadratic >= 0) {
		float tmp_1 = (-2 * (ray.direction->dot(ray.start)) + sqrtf(quadratic)) / (2 * ray.direction->mag() * ray.direction->mag());
		float tmp_2 = (-2 * (ray.direction->dot(ray.start)) - sqrtf(quadratic)) / (2 * ray.direction->mag() * ray.direction->mag());
		if (tmp_1 >= 5E-5 && tmp_2 >= 5E-5) {
			result.t = min(tmp_1, tmp_2);
		}
		else if (tmp_1 >= 5E-5 || tmp_1 >= 5E-5) {
			result.t = max(tmp_1, tmp_2);
		}

		if (result.t != std::numeric_limits<float>::infinity()) {
			Vector3 tmp_direction = ray.direction->multiply(result.t);
			*(result.point) = ray.start->add(&tmp_direction);
			*(result.normal) = *(result.point);
		}
	}

	//Transforms normals from local to world coords
	ray.transform(position, rotation, scale);
	result.point->transform(position, rotation, scale);
	result.normal->divide(scale);
	result.normal->normalize();

	return result;
}