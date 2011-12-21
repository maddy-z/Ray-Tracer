#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "Object3D.h"
#include "Triangle.h"

#include "Vector.h"
#include "Matrix.h"


bool Triangle::intersect(const Ray & r, Hit & h, float tmin)

{	

	const Vec3f & e1 = b - a;
	const Vec3f & e2 = c - a;

	const Vec3f & ori = r.getOrigin();
	const Vec3f & dir = r.getDirection();

	// P
	Vec3f P;
	Vec3f::Cross3(P, dir, e2);
	
	float det = e1.Dot3(P);

	Vec3f T; 
	if ( det > 0 ) {
		T = ori - a;
	}
	else {
		T = a - ori;
		det = -det;
	}

	float u = T.Dot3(P);
	if ( u < 0.0f || u > det )
		return false;

	// Q
	Vec3f Q;
	Vec3f::Cross3(Q, T, e1);

	float v = dir.Dot3(Q);
	if ( v < 0.0f || u + v > det )
		return false;

	float tTmp = e2.Dot3(Q) / det;
	if (tTmp > 1.0e-6 && tTmp > tmin && tTmp < h.getT()) 
	{
		normal.Normalize();
		h.set(tTmp, material, normal, r);

		return true;
	}
	else 
		return false;

}