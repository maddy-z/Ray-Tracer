#ifndef _RAY_H_
#define _RAY_H_

#include <string>
#include <iostream>
using namespace std;

#include "Global.h"
#include "Vector.h"

// Ray class mostly copied from Peter Shirley and Keith Morley

// ====================================================================
// ====================================================================

class Ray

{
	friend ostream & operator << (ostream & os, const Ray & r);

public:

	// CONSTRUCTOR & DESTRUCTOR
	Ray () {}
	Ray (const Vec3f &dir, const Vec3f &orig) {
		direction = dir;
		origin = orig; }
	Ray (const Ray &r) { *this = r; }

	virtual ~Ray() {}

	// ACCESSORS
	const Vec3f& getOrigin() const { return origin; }
	const Vec3f& getDirection() const { return direction; }
	Vec3f pointAtParameter(float t) const { return origin + direction * t; }

	virtual std::string toString()
	{
		return string("Ray:\n")
			 + string("\tDirection = ") + direction.toString() + NEW_LINE_CHAR
			 + string("\tOrigin = ") + origin.toString() + NEW_LINE_CHAR;
	}

private:

	// REPRESENTATION
	Vec3f direction;
	Vec3f origin;
};


inline ostream & operator << (ostream & os, const Ray & r) 
{
	os << "Ray < " << r.getOrigin() << ", " << r.getDirection() << " >";
	return os;
}

// ======================================================================
// ======================================================================

#endif
