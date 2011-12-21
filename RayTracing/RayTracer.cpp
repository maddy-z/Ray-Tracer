#include <cmath>
#include <cfloat>

#include "Vector.h"
#include "Ray.h"
#include "Hit.h"

#include "RayTracer.h"

// ===============================
//  Doing Ray Tracing Recursively
// ===============================

Vec3f RayTracer::traceRay(Ray & ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit & hit) const
{

	assert ( sparser != NULL );
	assert ( tmin >= 0.0f );
	assert ( bounces >= 0 );

	if ( bounces > max_bounces || weight < min_weight )
		return Vec3f ( 0.0f, 0.0f, 0.0f );

	// =========================================
	//  Whether Ray is intersected with objects
	// =========================================

	Group * group = sparser -> getGroup();
	if ( group -> intersect(ray, hit, tmin) == false )
		return sparser->getBackgroundColor();

	const Material * hitMaterial = hit.getMaterial();
	const Vec3f & hitPoint = hit.getIntersectionPoint();
	
	Vec3f & hitNormal = hit.getNormal(); hitNormal.Normalize();
	Vec3f rayDir = ray.getDirection(); rayDir.Normalize();

	const Vec3f & ambColor = sparser -> getAmbientLightColor();
	const Vec3f & diffColor = hitMaterial -> getDiffuseColor();
	Vec3f shadeColor(ambColor[0] * diffColor[0], ambColor[1] * diffColor[1], ambColor[2] * diffColor[2]);

	// =========================
	//  Calculate Light Shading
	// =========================

	Light ** lights = sparser -> getLights();
	for (int i = 0; i < sparser -> getLightNum(); ++i) 
	{
		Vec3f dirToLight, lightColor;

		lights[i] -> getIllumination(hitPoint, dirToLight, lightColor);

		Hit shadowHit(FLT_MAX, NULL, Vec3f(0.0f, 1.0f, 0.0f));
		Ray lightRay(dirToLight, hitPoint);
		
		// If shadow intersects with object
		if ( group -> intersect(lightRay, shadowHit, tmin) && shadowHit.getT() < lights[i] -> getDistance(hitPoint) ) {
			continue;
		} else {
			shadeColor += hitMaterial -> shade ( ray, hit, dirToLight, lightColor );
		}
	}

	float t1 = hitNormal.Dot3(rayDir);

	// =================================
	//  Calculate Mirror Reflective Ray
	// =================================
	
	if ( hit.getMaterial() -> isMirror() ) 
	{
		Vec3f mirrorDir;

		if ( t1 < 0.0f ) {
			mirrorDir = getMirrorDirection(hitNormal, rayDir);
		} else { 
			mirrorDir = getMirrorDirection((-1)*(hitNormal), rayDir);
		}

		Ray mirrorRay(mirrorDir, hitPoint + EPSILON * mirrorDir);
		Hit mirrorHit(FLT_MAX, NULL, Vec3f(0.0f, 1.0f, 0.0f));

		Vec3f mirrorShade = traceRay ( mirrorRay, tmin, bounces + 1, weight, indexOfRefraction, mirrorHit );
		Vec3f mirrorColor = hitMaterial->getReflectiveColor();

		shadeColor += Vec3f(mirrorShade[0] * mirrorColor[0], mirrorShade[1] * mirrorColor[1], mirrorShade[2] * mirrorColor[2]);
	}

	// ========================
	//  Calculate Transmit Ray
	// ========================
	
	if ( hit.getMaterial() -> isTransparent() )
	{
		Vec3f transmittedDir;
		
		float t2 = hitMaterial->getIndexOfRefraction();

		bool hasTransmitLight = false;
		if (t1 < 0.0f) { 
			hasTransmitLight = getTransmittedDirection(hitNormal, rayDir, indexOfRefraction, t2, transmittedDir);
		} else {
			hasTransmitLight = getTransmittedDirection((-1)*hitNormal, rayDir, t2, indexOfRefraction, transmittedDir);
		}

		if ( hasTransmitLight )
		{
			// cout << transmittedDir.toString() << endl;

			Ray transRay(transmittedDir, hitPoint + EPSILON * transmittedDir);
			Hit transHit(FLT_MAX, NULL, Vec3f(0.0f, 1.0f, 0.0f));

			const Vec3f & transShade = traceRay( transRay, tmin, bounces + 1, weight, indexOfRefraction, transHit );
			const Vec3f & transColor = hitMaterial->getTransparentColor();

			shadeColor += Vec3f(transShade[0] * transColor[0], transShade[1] * transColor[1], transShade[2] * transColor[2]);
		}
	}

	return shadeColor;

}


Vec3f RayTracer::getMirrorDirection(const Vec3f & normal, const Vec3f & incoming)
{
	Vec3f ref = incoming - 2 * incoming.Dot3(normal) * normal; ref.Normalize();
	return ref;
}


bool RayTracer::getTransmittedDirection(const Vec3f & normal, const Vec3f & incoming, float index_i, float index_t, Vec3f & transmitted)
{
	if (index_i == index_t) {
		transmitted = incoming; transmitted.Normalize();
		return true;
	}

	float index_r = index_i / index_t;

	float t1 = (-1) * (normal.Dot3(incoming));
	float t2 = 1 - index_r * index_r * (1 - t1 * t1);
	
	if (t2 > 0.0f) 
	{
		float t3 = index_r * t1 - sqrt( t2 );

		Vec3f transmittedDir = t3 * normal + index_r * incoming; transmittedDir.Normalize();
		transmitted = transmittedDir;

		return true;
	} else {
		return false;
	}
}