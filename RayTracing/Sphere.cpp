#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <iostream>
using namespace std;

#include "Sphere.h"

#include "Vector.h"
#include "Hit.h"
#include "Ray.h"

// ========
//  Sphere
// ========

bool Sphere::intersect(const Ray & r, Hit & h, float tmin)
{
	Vec3f D = r.getDirection();
	Vec3f R = r.getOrigin() - center;

	float a = D.Dot3(D);
	float b = 2 * ( D.Dot3(R) );
	float c = R.Dot3(R) - radius * radius;
	
	if (b * b < 4 * a * c)
		return false;

	float d = sqrt ( b * b - 4 * a * c );
	float t1 = ( -b + d ) / ( 2 * a );
	float t2 = ( -b - d ) / ( 2 * a );
	float t;

	if (t1 <= 0.0f && t2 <= 0.0f) 
		return false;
	else if (t1 > 0.0f && t2 <= 0.0f)
		t = t1;
	else if (t2 > 0.0f && t1 <= 0.0f)
		t = t2;
	else 
		t = (t1 < t2) ? t1 : t2;

	Vec3f interPoint = r.getOrigin() + t * D;

	if ( t > 1.0e-5 && t > tmin && t < h.getT() )
	{
		Vec3f n = interPoint - center;
		n.Normalize();

		h.set(t, material, n, r);
		return true;
	}
	
	return false;
}
