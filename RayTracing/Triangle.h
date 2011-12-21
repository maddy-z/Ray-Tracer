#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Object3D.h"
#include "Vector.h"
#include "Ray.h"
#include "Hit.h"


// ==========
//  Triangle
// ==========

class Triangle : public Object3D
{

public:

	Triangle(Vec3f & x, Vec3f & y, Vec3f & z, Material * material): Object3D(material), a(x), b(y), c(z) 
	{
		normal = calcNormal(a, b, c); normal.Normalize(); 
	}

	virtual ~Triangle() {};
	virtual bool intersect(const Ray & r, Hit & h, float tmin);

	virtual std::string toString()
	{
		return string("Triangle:\n")
			 + string("\tVertex a = ") + a.toString() + NEW_LINE_CHAR
			 + string("\tVertex b = ") + b.toString() + NEW_LINE_CHAR
			 + string("\tVertex c = ") + c.toString() + NEW_LINE_CHAR
			 + string("\tNormal = ") + normal.toString() + NEW_LINE_CHAR;
	}

private:

	Vec3f a, b, c;
	Vec3f normal;

};

#endif