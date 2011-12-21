#ifndef _PLANE_H_
#define _PLANE_H_

#include "Vector.h"
#include "Material.h"

#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"

#include <cassert>


/****************************
 *
 * Class Of Plane:
 * In the form of P * N = d
 *
 ****************************/

class Plane : public Object3D
{

public:

	Plane(Vec3f & n, float d, Material * material) : Object3D(material), normal(n), offset(d) { normal.Normalize(); }
	Plane(Vec3f & a, Vec3f & b, Vec3f & c, float d, Material * material) : Object3D(material), offset(d) { normal = calcNormal(a, b, c); normal.Normalize(); }

	// Plane(Vec3f &n, float d, Vec3f &color) : Object3D(color), normal(n), offset(d) { normal.Normalize(); }
	// Plane(Vec3f &a, Vec3f &b, Vec3f &c, float d, Vec3f &color) : Object3D(color), offset(d) { normal = calcNormal(a, b, c); normal.Normalize(); }

	virtual ~Plane() {}

	virtual bool intersect(const Ray & r, Hit & h, float tmin);

	virtual std::string toString()
	{
		char str[30];
		sprintf(str, "%f", offset);

		return string("Plane:\n")
			 + string("\tNormal = ") + normal.toString() + NEW_LINE_CHAR
			 + string("\tOffset = ") + string(str) + NEW_LINE_CHAR + NEW_LINE_CHAR
			 + material->toString();
	}

protected:

	Vec3f normal;
	float offset;

};

#endif