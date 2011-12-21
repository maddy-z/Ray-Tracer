#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>

#include "Camera.h"
#include "OrthographicCamera.h"
#include "Vector.h"
#include "Ray.h"


// ===========================================================
// ===========================================================

Ray OrthographicCamera::generateRay(const Vec2f & point)
{

	assert (point[0] >= 0.0f && point[0] <= 1.0f);
	assert (point[1] >= 0.0f && point[1] <= 1.0f);

	float hScale = size * (point.x() - 0.5f), uScale = size * (point.y() - 0.5);
	Vec3f h = horizontal, u = up;
	h.Normalize(); u.Normalize();

	h.Scale(uScale, uScale, uScale); 
	u.Scale(hScale, hScale, hScale);

	Vec3f origin = center + h + u;
	
	Ray r(direction, origin);
	return r;

}


void OrthographicCamera::initialize()
{
	direction.Normalize();
	up.Normalize();

	float temp = direction.Dot3(up);
	if (temp > 1e-6 || temp < (-1) * 1e-6)                        // if not ortho to direction vector ~~
	{
		std::cout << "WARNING:\tCamera's Up Vector is not orthogonal to Direction Vector." << endl;
		std::cout << "Start to adjust Camera's Up Vector" << endl;

		Vec3f::Cross3 ( horizontal, direction, up );
		horizontal.Normalize();

		Vec3f::Cross3 ( up, horizontal, direction );
		up.Normalize();

		std::cout << "End adjusting Camera's Up Vector" << endl;

		return;
	}																// Finish orthoing to the direction vector ~~~

	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	return;

}