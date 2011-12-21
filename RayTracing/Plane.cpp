#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <iostream>
using namespace std;

#include "Object3D.h"
#include "Plane.h"
#include "Vector.h"

bool Plane::intersect(const Ray & r, Hit & h, float tmin)
{
	assert (tmin >= 0.0f);

	Vec3f org = r.getOrigin();
	Vec3f dir = r.getDirection();

	if (dir.Dot3(normal) < 1.0e-7 && dir.Dot3(normal) > -1.0e-7) {
		return false;
	}					// Appromately parrell to plane

	float tempT = (offset - org.Dot3(normal)) / dir.Dot3(normal);
	if (tempT <= 1e-6) {
		return false;
	}
	else if (tempT >= tmin && tempT < h.getT()) 
	{
		// Update Hit Point
		normal.Normalize();
		h.set(tempT, material, normal, r);

		return true;
	}
	
	return false;
}