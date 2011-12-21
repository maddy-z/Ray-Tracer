#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

// ===============================
// Header Files for Camera Classes
// ===============================
#include "Camera.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

// ================================
// Header Files for Utility Classes
// ================================
#include "Global.h"
#include "Vector.h"
#include "Matrix.h"
#include "Image.h"
#include "Scene_Parser.h"

// ==================================
// Header Files for Object 3D Classes
// ==================================
#include "Object3D.h"
#include "Group.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Transform.h"

#include "Ray.h"
#include "Light.h"
#include "Hit.h"
#include "Material.h"

#endif