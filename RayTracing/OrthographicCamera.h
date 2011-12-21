#ifndef _ORTHOGRAPHIC_CAMERA_H_
#define _ORTHOGRAPHIC_CAMERA_H_

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
using namespace std;

#include "Camera.h"
#include "Vector.h"
#include "Matrix.h"

class OrthographicCamera : public Camera

{
	friend ostream & operator << (ostream & os, const OrthographicCamera & orthoCamera);

public:

	OrthographicCamera( Vec3f cen = Vec3f(0.0f, 0.0f, 0.0f), 
						Vec3f dir = Vec3f(0.0f, 0.0f, 1.0f),
						Vec3f u = Vec3f(0.0f, 1.0f, 0.0f),
						float s = 10.0f) : Camera(cen, dir, u), size(s)
	{
		assert (size > 0.0f);
		assert (!(direction[0] == 0.0f && direction[1] == 0.0f && direction[2] == 0.0f));
		assert (!(up[0] == 0.0f && up[1] == 0.0f && up[2] == 0.0f));

		initialize(); 
	}

	virtual ~OrthographicCamera() {}
	virtual Ray generateRay(const Vec2f & point);

	float getSize() const { return size; }
	void setSize(float s) { if (s > 0) size = s; }

	virtual std::string toString()
	{
		char str[20];
		sprintf(str, "%f", size);

		return string("OrthographicCamera:\n")
			 + string("\tCenter = ") + center.toString() + NEW_LINE_CHAR
			 + string("\tDirection = ") + direction.toString() + NEW_LINE_CHAR
			 + string("\tUp = ") + up.toString() + NEW_LINE_CHAR
			 + string("\tHorizontal = ") + horizontal.toString() + NEW_LINE_CHAR
			 + string("\tSize = ") + string(str) + NEW_LINE_CHAR;
	}

protected:
	float size;

private:

	// Initialization Function
	void initialize();

};


inline ostream & operator << (ostream & os, const OrthographicCamera & ogCamera)
{
	os << (Camera *)(&ogCamera)
	   << "\tSize:\t" << ogCamera.size << endl;
	
	return os;
}

#endif