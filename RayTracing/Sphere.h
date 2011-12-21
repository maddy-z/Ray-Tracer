#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <cassert>

#include "Object3D.h"
#include "Vector.h"
#include "Material.h"

class Sphere : public Object3D
{

public:

	Sphere( Vec3f c, float r = 1.0f, Material * material = NULL): 
			Object3D(material), 
			center(c), 
			radius(r) 
	{ assert (r > 0); }

	/*Sphere( Vec3f c, float r = 1.0f, Vec3f color = Vec3f(1.0f, 1.0f, 1.0f)): 
			Object3D(color), 
			center(c), 
			radius(r) 
	{ assert (r > 0); } */
	
	virtual ~Sphere() {}

	virtual bool intersect(const Ray &r, Hit &h, float min);

	Vec3f getCenter() const { return center; }
	float getRadius() const { return radius; }

	void setCenter(Vec3f c) { center = c; }
	void setRadius(float r) { radius = r; }

	virtual std::string toString()
	{
		char str[20];
		sprintf(str, "%f", radius);

		return string("Sphere:\n")
			 + string("\tCenter = ") + center.toString() + NEW_LINE_CHAR
			 + string("\tRadius = ") + string(str) + NEW_LINE_CHAR + NEW_LINE_CHAR
			 + material->toString();
	}

protected:

	Vec3f center;
	float radius;

};

inline ostream& operator << (ostream & os, Sphere & s)
{
	os << "Sphere:\n" << "\tCenter = " << s.getCenter() << endl
					  << "\tRadius = " << s.getRadius() << endl;
	return os;
}

#endif