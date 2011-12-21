#include <cassert>

#include "Object3D.h"
#include "Group.h"

class Object3D;

Group::Group(int num) : objs_num(num)
{
	if (num == 0) {
		objs = NULL;
		return;
	}

	assert (num > 0);
	objs = new Object3D * [num];

	return;
}

Group::~Group()
{
	for (int i = 0; i < objs_num; ++i) {
		delete objs[i];
	}

	if (objs != NULL)
		delete [] objs;
}

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
	bool iFlag = false;

	for (int i = 0; i < objs_num; ++i) {
		if ( objs[i]->intersect(r, h, tmin) )
			iFlag = true;
	}

	return iFlag;
}

void Group::addObject(int index, Object3D *obj)
{
	assert (index >= 0 && index < objs_num);

	objs[index] = obj;
	return;
}