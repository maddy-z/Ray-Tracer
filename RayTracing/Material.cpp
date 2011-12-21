#include <cmath>

#include "Vector.h"
#include "Hit.h"
#include "Ray.h"
#include "Material.h"

Vec3f PhongMaterial::shade( const Ray & ray, 
							const Hit & hit, 
							const Vec3f & dirToLight, 
							const Vec3f & lightColor ) const
{	
	Vec3f n = hit.getNormal(); n.Normalize();
	Vec3f l = dirToLight; l.Normalize();

	if ( n.Dot3(l) < 0.0f ) {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

	float diff_coff = n.Dot3(l);
	Vec3f diff_comp = diffuseColor * diff_coff;

	// Vec3f view = hit.getIntersectionPoint() - ray.getOrigin();
	Vec3f r = 2 * l.Dot3(n) * n - l; r.Normalize();
	Vec3f v = ray.getOrigin() - hit.getIntersectionPoint(); v.Normalize();
	
	Vec3f spec_comp(0.0f, 0.0f, 0.0f);
	float spec_coff = v.Dot3(r);
	if (spec_coff > 0.0f)
		spec_comp += specularColor * pow(spec_coff, exponent);

	const Vec3f &sum = diff_comp + spec_comp;
	return Vec3f(sum[0] * lightColor[0], sum[1] * lightColor[1], sum[2] * lightColor[2]);

}