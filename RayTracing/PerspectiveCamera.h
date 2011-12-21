#ifndef _PERSPECTIVE_CAMERA_H_
#define _PERSPECTIVE_CAMERA_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "Camera.h"
#include "Vector.h"
#include "Matrix.h"
#include "Ray.h"

// =============================
//  Class Of Perspective Camera
// =============================

class PerspectiveCamera : public Camera

{
	friend ostream & operator << (ostream & os, const PerspectiveCamera & persCamera);

public:

	PerspectiveCamera(Vec3f & cen = Vec3f(0.0f, 0.0f, 0.0f), 
					  Vec3f & dir = Vec3f(0.0f, 0.0f, 1.0f), 
					  Vec3f & u = Vec3f(0.0f, 1.0f, 0.0f), 
					  float a = 90) : Camera(cen, dir, u),
									  angle(a)
	{
		assert (a > 0);
		assert (!(direction[0] == 0.0f && direction[1] == 0.0f && direction[2] == 0.0f));
		assert (!(up[0] == 0.0f && up[1] == 0.0f && up[2] == 0.0f));

		initialize();
	}

	// Set Attribute -- Angle
	double getAngle() const { return angle; }
	void setAngle(float ang) { angle = ang; }

	virtual ~PerspectiveCamera() {}
	virtual Ray generateRay(const Vec2f & point);

	virtual std::string toString()
	{
		char str[30];
		sprintf(str, "%f", angle);

		return string("PerspectiveCamera:\n")
			 + string("\tCenter = ") + center.toString() + NEW_LINE_CHAR
			 + string("\tDirection = ") + direction.toString() + NEW_LINE_CHAR
			 + string("\tUp = ") + up.toString() + NEW_LINE_CHAR
			 + string("\tHorizontal = ") + horizontal.toString() + NEW_LINE_CHAR
			 + string("\tAngle = ") + string(str) + NEW_LINE_CHAR;
	}


protected:

	float angle;

private:

	void initialize();

};


inline ostream & operator << (ostream & os, const PerspectiveCamera & persCamera)
{
	os << (Camera *)(&persCamera);
	os << "\tAngle = " << persCamera.angle << endl;

	return os;
}

#endif