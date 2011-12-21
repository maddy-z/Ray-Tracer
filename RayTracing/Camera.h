#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
using namespace std;

#include "Vector.h"
#include "Ray.h"

// ==============================================
// ==============================================

class Camera
{
	friend ostream & operator << (ostream & os, const Camera * camera);

public:

	Camera( Vec3f cen = Vec3f(0.0f, 0.0f, 0.0f), 
			Vec3f dir = Vec3f(0.0f, 0.0f, 1.0f),
			Vec3f u = Vec3f(0.0f, 1.0f, 0.0f) ) : center(cen), direction(dir), up(u)
	{}

	virtual ~Camera() {}

	// Pure Virtual Function
	virtual Ray generateRay(const Vec2f &point) = 0;

	virtual std::string toString()
	{
		return string("Camera:\n")
			 + string("\tCenter = ") + center.toString() + NEW_LINE_CHAR
			 + string("\tDirection = ") + direction.toString() + NEW_LINE_CHAR
			 + string("\tUp = ") + up.toString() + NEW_LINE_CHAR
			 + string("\tHorizontal = ") + horizontal.toString() + NEW_LINE_CHAR;
	}

protected:
	
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;

private:
	
};

inline ostream & operator << (ostream & os, const Camera * camera)
{
	os << "Camera:"			<< endl 
	   << "\tCenter = "		<< camera->center << endl 
	   << "\tDirection = "	<< camera->direction << endl
	   << "\tUp = "			<< camera->up << endl;

	return os;
}

#endif