#ifndef _RAY_TRACER_H_
#define _RAY_TRACER_H_

#include "Scene_Parser.h"

class Hit;
class Ray;

// ====================================
// ====================================

#define EPSILON 1.0e-4

class RayTracer
{

public:
	
	RayTracer( SceneParser * sp,
			   int max_b,
			   float min_wei) : sparser(sp), max_bounces(max_b), min_weight(min_wei)
	{
		assert (max_b >= 0);
		assert (min_weight > 0.0f);
	}

	virtual ~RayTracer() {}

	Vec3f traceRay(Ray & ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit & hit) const;
	
	static Vec3f getMirrorDirection(const Vec3f & normal, const Vec3f & incoming);
	static bool getTransmittedDirection(const Vec3f & normal, const Vec3f & incoming, float index_i, float index_t, Vec3f & transmitted);

private:

	SceneParser * sparser;
	int max_bounces;
	float min_weight;

};

#endif