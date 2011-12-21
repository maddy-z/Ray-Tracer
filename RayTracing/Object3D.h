#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

#include "Vector.h"
#include "Material.h"

class Ray;
class Hit;

// =================================================
// =================================================

class Object3D
{

	friend ostream & operator << (ostream & os, const Object3D * obj);

public:

	Object3D(Material * m = NULL) : material(m) {}
	virtual ~Object3D() { material = NULL; }

	virtual bool intersect(const Ray & r, Hit & h, float tmin) = 0;

	const Material * getMaterial() const { return material; }
	void setMaterial(Material * mat) { material = mat; } 

	virtual std::string toString()
	{
		string header = "Object3D:\n";

		if (material != NULL) {
			return header + material->toString();
		} else {
			return header;
		}
	}

protected:
	
	Material * material;

};

inline ostream & operator << (ostream & os, const Object3D * obj)
{
	os << "Object 3D Information:" << endl;
	// TODO:

	return os;
}

#endif