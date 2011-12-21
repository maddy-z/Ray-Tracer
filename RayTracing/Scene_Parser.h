#ifndef _SCENE_PARSER_H_
#define _SCENE_PARSER_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

// =================================
//  Header Files for Camera Classes
// =================================
#include "Camera.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

// ==================================
//  Header Files for Utility Classes
// ==================================
#include "Vector.h"
#include "Matrix.h"
#include "Image.h"

// ====================================
//  Header Files for Object 3D Classes
// ====================================
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

#define  MAX_PARSER_TOKEN_LENGTH  100


// ===================================================
// ===================================================

class SceneParser 

{
	friend ostream & operator << (ostream & os, const SceneParser & sparser);

public:

	// CONSTRUCTORS, DESTRUCTOR & INITIALIZE
	SceneParser();
	SceneParser(const char * filename);
	
	virtual ~SceneParser();

	// ACCESSORS
	Group * getGroup() const { return group; }
	Camera * getCamera() const { return camera; }
	Light ** getLights() const { return lights; }
	Material ** getMaterials() const { return materials; }

	Vec3f getBackgroundColor() const { return background_color; }
	Vec3f getAmbientLightColor() const { return ambient_light; }
	
	int getLightNum() const { return num_lights; }
	int getMaterialNum() const { return num_materials; }

	virtual std::string toString();

protected:

	// HELPER FUNCTIONS
	void initialize();

	int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);

	void parseFile();
	void parseOrthographicCamera();
	void parsePerspectiveCamera();
	void parseLights();
	void parseBackground();
	void parseMaterials();

	Object3D* parseObject(char token[MAX_PARSER_TOKEN_LENGTH]);
	Group* parseGroup();
	Sphere* parseSphere();
	Plane* parsePlane();
	Triangle* parseTriangle();
	Group* parseTriangleMesh();
	Transform* parseTransform();
	DirectionalLight* parseDirectionalLight();
	PointLight* parsePointLight();

	Material* parseNormalMaterial();
	PhongMaterial* parsePhongMaterial();

	Vec3f readVec3f();
	float readFloat();
	int readInt();

private:

	// REPRESENTATION
	Camera * camera;
	Group * group;
	
	Vec3f ambient_light;
	Vec3f background_color;
	// Vec3f current_object_color;
	
	Light ** lights;
	int num_lights;

	Material ** materials;
	int num_materials;

	Material *currentMaterial;
	FILE * file;

};

// ========================================================
// ========================================================

#endif
