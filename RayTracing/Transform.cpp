#include "Object3D.h"
#include "Transform.h"
#include "Hit.h"
#include "Ray.h"

#include "Vector.h"
#include "Matrix.h"

// ============================================
//  Complicated Implemention Of Transformation
// ============================================

Transform::Transform(Matrix & m, Object3D * obj) : object(obj), transformMat(m)
{
	assert (obj != NULL);
}

bool Transform::intersect(const Ray & r, Hit & h, float tmin)
{
	// Need to Transform Ray before do intersection

	Matrix reverseMat = transformMat;
	reverseMat.Inverse();
	
	// cout << "Original Origin:\t" << r.getOrigin() << endl;
	// cout << "Original Direction:\t" << r.getDirection() << endl;

	Vec4f aug_origin(r.getOrigin(), 1);
	Vec3f aug_dir = r.getDirection();

	reverseMat.Transform(aug_origin);
	reverseMat.TransformDirection(aug_dir);

	// cout << "Now Origin:\t" << r.getOrigin() << endl;
	// cout << "Now Direction:\t" << r.getDirection() << endl;

	// aug_dir.Normalize();
	Ray transRay(aug_dir, Vec3f(aug_origin[0], aug_origin[1], aug_origin[2]));
	if ( !( object -> intersect(transRay, h, tmin) ) )
		return false;
	
	// After transforming Ray, we need to transform Normal
	Matrix transposeRevMat = reverseMat;
	transposeRevMat.Transpose();
	Vec3f hn = h.getNormal();
	transposeRevMat.Transform(hn);
	hn.Normalize();

	h.set(h.getT(), h.getMaterial(), hn, r);

	return true;

}


std::string Transform::toString()
{
	return string("Transform:\n")
		 + string("\tObject3D = ") + object->toString();
}