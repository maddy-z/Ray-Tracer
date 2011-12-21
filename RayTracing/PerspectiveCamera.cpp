#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>

#include "Camera.h"
#include "PerspectiveCamera.h"
#include "Ray.h"
#include "Vector.h"
#include "Matrix.h"

#define PI 3.1415926

// ==========================================================================
//  Implemention of Virtual Function -- Ray generateRay(const Vec2f & point)
// ==========================================================================

Ray PerspectiveCamera::generateRay(const Vec2f & point)

{

	assert ( point[0] >= 0.0f && point[0] <= 1.0f );
	assert ( point[1] >= 0.0f && point[1] <= 1.0f );
	
	assert ( angle >= 0.0f && angle <= 180.0f );

	float size = 2.0f * tan ( angle / 2.0f );

	float hScale = size * ( point.x() - 0.5f );
	float uScale = size * ( point.y() - 0.5f );

	horizontal.Normalize();
	up.Normalize();
	direction.Normalize();
	
	const Vec3f & cen = center + direction;
	const Vec3f & pt = cen + horizontal * uScale + up * hScale;
	
	Vec3f & dir = pt - center; dir.Normalize();

	Ray ray(dir, center);
	return ray;

}


void PerspectiveCamera::initialize()
{
	direction.Normalize();
	up.Normalize();

	float temp = direction.Dot3(up);
	if (temp > 1e-6 || temp < (-1) * 1e-6)                        // if orthonal to direction vector ~~~
	{                     
		std::cout << "WARNING:\tCamera's Up Vector is not orthogonal to Direction Vector." << endl;
		std::cout << "Start to adjust Camera's Up Vector" << endl;

		Vec3f::Cross3(horizontal, direction, up);
		horizontal.Normalize();

		Vec3f::Cross3(up, horizontal, direction);
		up.Normalize();

		std::cout << "End adjusting Camera's Up Vector" << endl;

		return;
	}				 // Finish orthoing to the direction vector ~~~

	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	return;
}