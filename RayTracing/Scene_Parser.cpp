#include <cstdio>
#include <cstdlib>

#include <string>
#include <iostream>
using namespace std;

#include "Common.h"
#include "Scene_Parser.h"

#define		M_PI					3.1415926
#define		DegreesToRadians(x)		( (M_PI * x) / 180.0f )


// =======================================
//  CONSTRUCTORS, DESTRUCTOR & INITIALIZE
// =======================================

SceneParser::SceneParser()
{
	initialize();
}

SceneParser::SceneParser(const char * filename) 
{
	initialize();

	// open the file
	assert(filename != NULL);

	const char * ext = &(filename[strlen(filename) - 4]);
	assert (!strcmp(ext, ".txt"));

	file = fopen(filename, "r");
	assert (file != NULL);

	// parse the scene
	parseFile();

	// close the file
	fclose(file); 
	file = NULL;

	currentMaterial = NULL;
}

SceneParser::~SceneParser() 
{
	if (group != NULL)  delete group;
	if (camera != NULL) delete camera;

	for (int i = 0; i < num_lights; ++i) {
		delete lights[i]; }
	delete [] lights;

	for (int i = 0; i < num_materials; ++i) {
		delete materials[i]; }
	delete [] materials;
}

void SceneParser::initialize()
{
	// initialize some reasonable default values
	group = NULL;
	camera = NULL;
	
	lights = NULL;
	num_lights = 0;
	
	materials = NULL;
	num_materials = 0;

	background_color = Vec3f(0.0f, 0.0f, 0.0f);
	ambient_light = Vec3f(0.5f, 0.5f, 0.5f);
	
	currentMaterial = NULL;
	file = NULL;

}


// ========================================================
// ========================================================

void SceneParser::parseFile() 
{
	//
	// At the top level, the scene can have a camera, 
	// background color and a group of objects.
	//
	// ( We will add lights and other things in future assignments. )
	//
	
	char token[MAX_PARSER_TOKEN_LENGTH];

	while (getToken(token)) 
	{
		if (!strcmp(token, "OrthographicCamera")) {
			parseOrthographicCamera();
		} else if (!strcmp(token, "PerspectiveCamera")) {
			parsePerspectiveCamera();
		} else if (!strcmp(token, "Background")) {
			parseBackground();
		} else if (!strcmp(token, "Lights")) {
			parseLights();
		} else if (!strcmp(token, "Group")) {
			group = parseGroup();
		} else if (!strcmp(token, "Materials")) {
			parseMaterials();
		} else {
			printf ("Unknown token in parseFile: '%s'\n", token);
			exit(0);
		}
	}

	return;
}

Group* SceneParser::parseGroup()
{
	//
	// Each group starts with an integer that specifies
	// the number of objects in the group;
	//
	// The material node sets the color of all objects which 
	// follow, until the next material node;
	//
	
	char token[MAX_PARSER_TOKEN_LENGTH];
	getToken(token); assert (!strcmp(token, "{"));

	// read in the number of objects
	getToken(token); assert (!strcmp(token, "numObjects"));
	int num_objects = readInt();

	Group * answer = new Group(num_objects);

	// read in the objects
	int count = 0;
	while (num_objects > count)
	{
		getToken(token);

		if (!strcmp(token, "MaterialIndex")) 
		{
			// materials don't count
			// parseMaterial();

			int index = readInt();
			currentMaterial = materials[index];
		}
		else 
		{
			// everything else does count
			
			Object3D *object = parseObject(token);
			assert (object != NULL);
			answer -> addObject(count, object);
			
			++count;
		}
	}
	
	getToken(token); assert (!strcmp(token, "}"));

	// return the group
	return answer;
}


Object3D * SceneParser::parseObject(char token[MAX_PARSER_TOKEN_LENGTH]) 
{
	// printf ("PARSE OBJECT %s\n", token);
	Object3D *answer = NULL;
	
	if (!strcmp(token, "Group")) {      
		answer = (Object3D *)(parseGroup());
	} else if (!strcmp(token, "Sphere")) {      
		answer = (Object3D *)(parseSphere());
	} else if (!strcmp(token, "Box")) {      
		// answer = (Object3D*)(parseBox());
	} else if (!strcmp(token, "Plane")) {      
		answer = (Object3D *)(parsePlane());
	} else if (!strcmp(token, "Triangle")) {
		answer = (Object3D *)(parseTriangle());
	} else if (!strcmp(token, "TriangleMesh")) {      
		answer = (Object3D *)(parseTriangleMesh());
	} else if (!strcmp(token, "Transform")) {      
		answer = (Object3D *)(parseTransform());
	} else {
		printf ("Unknown token in parseObject: '%s'\n", token);
		exit(0);
	}
	
	return answer;
}

// =================================================================
// =================================================================

void SceneParser::parseMaterials()
{
	char token[MAX_PARSER_TOKEN_LENGTH];
	getToken(token); assert (!strcmp(token, "{"));

	// Read in the number of objects
	getToken(token); assert (!strcmp(token, "numMaterials"));
	num_materials = readInt(); assert (num_materials > 0);
	materials = (Material **)(new Material*[num_materials]);

	int count = 0;
	while (num_materials > count)
	{
		getToken(token);

		if (!strcmp(token, "Material")) {
			materials[count++] = parseNormalMaterial();
		}
		else if (!strcmp(token, "PhongMaterial")) {
			materials[count++] = parsePhongMaterial();
		}
		else {
			printf ("Unknown token in parseGroup: '%s'\n", token);
			exit(0);
		}
	}

	getToken(token); assert (!strcmp(token, "}"));

	return;
}

Material* SceneParser::parseNormalMaterial() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];
	
	// change the current object color
	// scoping for the materials is very simplistic,
	// and essentially ignores any tree hierarchy

	getToken(token); assert (!strcmp(token, "{"));

	getToken(token); assert (!strcmp(token, "diffuseColor"));
	Vec3f diffuseColor = readVec3f();
	
	getToken(token); assert (!strcmp(token, "}"));

	// TODO:
	return NULL;
}

PhongMaterial* SceneParser::parsePhongMaterial()
{
	PhongMaterial *phongMaterial = new PhongMaterial();
	
	char token[MAX_PARSER_TOKEN_LENGTH];
	getToken(token); assert (!strcmp(token, "{"));

	// read in transformations: 
	// apply to the LEFT side of the current matrix ( so the first
	// transform in the list is the last applied to the object ).

	getToken(token);

	while (true) 
	{
		if (!strcmp(token, "diffuseColor")) {
			phongMaterial -> diffuseColor = readVec3f();
		} else if (!strcmp(token, "specularColor")) {
			phongMaterial -> specularColor = readVec3f();
		} else if (!strcmp(token, "exponent")) {
			phongMaterial -> exponent = readFloat();
		} else if (!strcmp(token, "transparentColor")) {
			phongMaterial -> transparentColor = readVec3f();
		} else if (!strcmp(token, "reflectiveColor")) {
			phongMaterial -> reflectiveColor = readVec3f();
		} else if (!strcmp(token, "indexOfRefraction")) {
			phongMaterial -> indexOfRefraction = readFloat();
		} else {
			// otherwise this must be an object,
			// and there are no more transformations
			// object = parseObject(token);
	
			break;
		}
    
		getToken(token);
	}
	
	// getToken(token);
	assert (!strcmp(token, "}"));

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	// CALLING ASSIGNMENT 2 CODE
	// create a new transform object and return it

	return phongMaterial;

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// =================================================================
// =================================================================

void SceneParser::parseLights() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];
	getToken(token); assert (!strcmp(token, "{"));

	// read in the number of objects
	getToken(token); assert (!strcmp(token, "numLights"));
	num_lights = readInt();
	lights = (Light **)(new Light*[num_lights]);

	// read in the objects
	int count = 0;
	while (num_lights > count) 
	{
		getToken(token);

		if (!strcmp(token, "DirectionalLight")) 
		{
			/*
			getToken(token); assert (!strcmp(token, "{"));
			getToken(token); assert (!strcmp(token, "direction"));
			
			Vec3f direction = readVec3f();
			getToken(token); assert (!strcmp(token, "color"));
			
			Vec3f color = readVec3f();
			getToken(token); assert (!strcmp(token, "}"));
			
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++
			// CALLING ASSIGNMENT 2 CODE
			lights[count] = new DirectionalLight(direction,color);
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++
			
			count++;
			*/

			lights[count++] = parseDirectionalLight();
		} else if (!strcmp(token, "PointLight")) {
			lights[count++] = parsePointLight();
		}
		else {
			printf ("Unknown token in parseGroup: '%s'\n", token);
			exit(0);
		}   
	}

	getToken(token); assert (!strcmp(token, "}"));

	return;
}

DirectionalLight* SceneParser::parseDirectionalLight()
{
	char token[MAX_PARSER_TOKEN_LENGTH];

	// read in the camera parameters
	getToken(token); assert ( !strcmp(token, "{") );

	getToken(token); assert ( !strcmp(token, "direction") );
	Vec3f dir = readVec3f();

	getToken(token); assert ( !strcmp(token, "color") );
	Vec3f color = readVec3f();

	getToken(token); assert ( !strcmp(token, "}") );

	return new DirectionalLight(dir, color);
}

PointLight* SceneParser::parsePointLight()
{
	char token[MAX_PARSER_TOKEN_LENGTH];

	// read in the camera parameters
	getToken(token); assert ( !strcmp(token, "{") );

	getToken(token); assert ( !strcmp(token, "position") );
	Vec3f pos = readVec3f();

	getToken(token); assert ( !strcmp(token, "color") );
	Vec3f color = readVec3f();
	
	PointLight *ptLight = new PointLight(pos, color);

	getToken(token);
	if ( !strcmp(token, "attenuation") ) 
	{
		ptLight->setAttenuation(readVec3f());
		getToken(token); assert ( !strcmp(token, "}"));
	} else {
		assert ( !strcmp(token, "}"));
	}

	return ptLight;
}

// ====================================================================
// ====================================================================

void SceneParser::parseOrthographicCamera() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];

	// read in the camera parameters
	getToken(token); assert ( !strcmp(token, "{") );
	getToken(token); assert ( !strcmp(token, "center") );
	Vec3f center = readVec3f();

	getToken(token); assert ( !strcmp(token, "direction") );
	Vec3f direction = readVec3f();

	getToken(token); assert ( !strcmp(token, "up") );
	Vec3f up = readVec3f();

	getToken(token); assert ( !strcmp(token, "size") );
	float size = readFloat();

	getToken(token); assert ( !strcmp(token, "}") );

	camera = new OrthographicCamera(center, direction, up, size);
}


void SceneParser::parsePerspectiveCamera() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];

	// read in the camera parameters
	getToken(token); assert (!strcmp(token, "{"));

	getToken(token); assert (!strcmp(token, "center"));
	Vec3f center = readVec3f();

	getToken(token); assert (!strcmp(token, "direction"));	
	Vec3f direction = readVec3f();

	getToken(token); assert (!strcmp(token, "up"));	
	Vec3f up = readVec3f();

	getToken(token); assert (!strcmp(token, "angle"));
	float angle_degrees = readFloat();

	float angle_radians = DegreesToRadians(angle_degrees);
	
	getToken(token); assert (!strcmp(token, "}"));

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	// CALLING ASSIGNMENT 2 CODE
	// create a new perspective camera and return it
	
	camera = new PerspectiveCamera(center, direction, up, angle_radians);
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
}


void SceneParser::parseBackground() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];
	
	// read in the background color
	getToken(token); assert (!strcmp(token, "{"));  
	
	while (1) 
	{
		getToken(token); 
	
		if (!strcmp(token, "}")) { 
			break;  
		} else if (!strcmp(token, "color")) {
			background_color = readVec3f();
		} else if (!strcmp(token, "ambientLight")) {
			ambient_light = readVec3f();
		} else {
			printf ("Unknown token in parseBackground: '%s'\n", token);
			assert(0);
		}
	}
}

// ====================================================================
// ====================================================================

Sphere * SceneParser::parseSphere() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];

	// read in the sphere parameters
	getToken(token); assert (!strcmp(token, "{"));
	
	getToken(token); assert (!strcmp(token, "center"));	
	Vec3f center = readVec3f();

	getToken(token); assert (!strcmp(token, "radius"));
	float radius = readFloat();

	getToken(token); assert (!strcmp(token, "}"));

	return new Sphere(center, radius, currentMaterial);

}


Plane* SceneParser::parsePlane() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];

	// read in the sphere parameters
	getToken(token); assert (!strcmp(token, "{"));
	
	getToken(token); assert (!strcmp(token, "normal"));
	Vec3f normal = readVec3f();
	
	getToken(token); assert (!strcmp(token, "offset"));
	float offset = readFloat();

	getToken(token); assert (!strcmp(token, "}"));

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	// CALLING ASSIGNMENT 2 CODE
	// create a new plane object and return it

	return new Plane(normal, offset, currentMaterial);

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
}

Triangle* SceneParser::parseTriangle() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];

	// read in the sphere parameters
	getToken(token); assert (!strcmp(token, "{"));

	getToken(token); assert (!strcmp(token, "vertex0"));	
	Vec3f v0 = readVec3f();

	getToken(token); assert (!strcmp(token, "vertex1"));
	Vec3f v1 = readVec3f();

	getToken(token); assert (!strcmp(token, "vertex2"));
	Vec3f v2 = readVec3f();

	getToken(token); assert (!strcmp(token, "}"));

	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	// CALLING ASSIGNMENT 2 CODE
	// create a new triangle object and return it
	
	return new Triangle(v0, v1, v2, currentMaterial);
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
}


Group* SceneParser::parseTriangleMesh() 
{
	char token[MAX_PARSER_TOKEN_LENGTH];
	char filename[MAX_PARSER_TOKEN_LENGTH];

	// get the filename
	getToken(token); assert (!strcmp(token, "{"));
	getToken(token); assert (!strcmp(token, "obj_file"));
	getToken(filename);
	getToken(token); assert (!strcmp(token, "}"));

	// read it once, get counts
	FILE *file = fopen(filename, "r");
	assert (file != NULL);
	
	int vcount = 0; int fcount = 0;
	while (1) {
		int c = fgetc(file);
		if (c == EOF) { 
			break;
		} else if (c == 'v') { 
			assert(fcount == 0); float v0, v1, v2;
			fscanf (file, "%f %f %f", &v0, &v1, &v2);
			vcount++; 
		} else if (c == 'f') {
			int f0, f1, f2;
			
			fscanf (file,"%d %d %d", &f0, &f1, &f2);
			fcount++;
		}	// otherwise, must be whitespace
	}
	
	// printf ("verts %d faces %d\n", vcount, fcount);
	fclose(file);

	// make arrays
	Vec3f *verts = new Vec3f[vcount];
	Group *answer = new Group(fcount);

	// read it again, save it
	file = fopen(filename, "r");
	assert (file != NULL);
	
	int new_vcount = 0; int new_fcount = 0;
	
	while (1) 
	{
		int c = fgetc(file);

		if (c == EOF) { 
			break;
		} else if (c == 'v') { 
			assert(new_fcount == 0); 
			
			float v0, v1, v2;
			fscanf (file, "%f %f %f", &v0, &v1, &v2);
			verts[new_vcount] = Vec3f(v0, v1, v2);
			new_vcount++;
		} else if (c == 'f') {
			assert (vcount == new_vcount);
			
			int f0, f1, f2;
			fscanf (file, "%d %d %d", &f0, &f1, &f2);
			
			// indexed starting at 1...
			assert (f0 > 0 && f0 <= vcount);
			assert (f1 > 0 && f1 <= vcount);
			assert (f2 > 0 && f2 <= vcount);

			// ++++++++++++++++++++++++++++++++++++++++++++++++++++
			// CALLING ASSIGNMENT 2 CODE
			// create a new triangle object and return it
			
			Triangle *t = new Triangle(verts[f0-1], verts[f1-1], verts[f2-1], currentMaterial);
			
			// ++++++++++++++++++++++++++++++++++++++++++++++++++++

			answer -> addObject(new_fcount, t);
			new_fcount++; 
		}	// otherwise, must be whitespace
	}
	
	delete [] verts;
	assert ( fcount == new_fcount );
	assert ( vcount == new_vcount );
	fclose ( file );

	return answer;
}


Transform * SceneParser::parseTransform()
{
	char token[MAX_PARSER_TOKEN_LENGTH];
	
	Matrix matrix; matrix.SetToIdentity();
	Object3D *object = NULL;
	getToken(token); assert (!strcmp(token, "{"));

	// read in transformations: 
	// apply to the LEFT side of the current matrix (so the first
	// transform in the list is the last applied to the object)

	getToken(token);

	while (1) 
	{
		if (!strcmp(token, "Scale")) {
			getToken(token); assert (!strcmp(token, "{"));
			matrix *= Matrix::MakeScale(readVec3f());
			getToken(token); assert (!strcmp(token, "}"));
		} else if (!strcmp(token, "XRotate")) {
			getToken(token); assert (!strcmp(token, "{"));
			matrix *= Matrix::MakeXRotation(DegreesToRadians(readFloat()));
			getToken(token); assert (!strcmp(token, "}"));
		} else if (!strcmp(token, "YRotate")) {
			getToken(token); assert (!strcmp(token, "{"));
			matrix *= Matrix::MakeYRotation(DegreesToRadians(readFloat()));
			getToken(token); assert (!strcmp(token, "}"));
		} else if (!strcmp(token, "ZRotate")) {
			getToken(token); assert (!strcmp(token, "{"));
			matrix *= Matrix::MakeZRotation(DegreesToRadians(readFloat()));
			getToken(token); assert (!strcmp(token, "}"));
		} else if (!strcmp(token, "Rotate")) {
			getToken(token); assert (!strcmp(token, "{"));
			Vec3f axis = readVec3f();
			float degrees = readFloat();
			matrix *= Matrix::MakeAxisRotation(axis,DegreesToRadians(degrees));
			getToken(token); assert (!strcmp(token, "}"));
		} else if (!strcmp(token, "Translate")) {
			getToken(token); assert (!strcmp(token, "{"));
			matrix *= Matrix::MakeTranslation(readVec3f());
			getToken(token); assert (!strcmp(token, "}"));
		} else if (!strcmp(token, "Matrix")) {
			Matrix matrix2; matrix2.SetToIdentity();
			getToken(token); assert (!strcmp(token, "{"));
			
			for (int j = 0; j < 4; ++j) 
			{
				for (int i = 0; i < 4; ++i) {
					float v = readFloat();
					matrix2.Set(i, j, v); 
				} 
			}
      
			getToken(token); assert (!strcmp(token, "}"));
			matrix = matrix2 * matrix;
		} else {
			// otherwise this must be an object,
			// and there are no more transformations
			object = parseObject(token);
			break;
		}

		getToken(token);
	}
	
	assert(object != NULL);
	getToken(token); assert (!strcmp(token, "}"));

	// +++++++++++++++++++++++++++++++++++++++++++++++
	// CALLING ASSIGNMENT 2 CODE
	// create a new transform object and return it

	return new Transform(matrix, object);

	// +++++++++++++++++++++++++++++++++++++++++++++++
}

std::string SceneParser::toString()
{
	string re = "Scene Parser Information:\n\n";
	
	// Output Background Information
	re += string("Background Color:\t") + background_color.toString() + NEW_LINE_CHAR;
	re += string("Ambient Light Color:\t") + ambient_light.toString() + NEW_LINE_CHAR;
	re += NEW_LINE_CHAR;

	// Output Camera Information
	re += camera -> toString();
	re += NEW_LINE_CHAR;

	// Output Lights Information
	char str1[10]; sprintf(str1, "%d", num_lights);
	re += string("Lights Information:\n");
	re += string("Light Number:\t") + string(str1) + NEW_LINE_CHAR;
	re += NEW_LINE_CHAR;
	for (int i = 0; i < num_lights; ++i) {
		re += lights[i]->toString();
		re += NEW_LINE_CHAR;
	}

	// Output Group Information
	re += group->toString();
	re += NEW_LINE_CHAR;

	// Output Materials Information
	char str2[10]; sprintf(str2, "%d", num_materials);
	re += string("Materials Information:\n");
	re += string("Material Number:\t") + string(str2) + NEW_LINE_CHAR;
	re += NEW_LINE_CHAR;
	for (int i = 0; i < num_materials; ++i) {
		re += materials[i]->toString();
		re += NEW_LINE_CHAR;
	}

	return re;
}


// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) 
{
	// for simplicity, tokens must be separated by whitespace
	assert (file != NULL);
  
	int success = fscanf(file, "%s ", token);
	if (success == EOF) {
		token[0] = '\0';
		return 0;
	}
	
	return 1;
}

Vec3f SceneParser::readVec3f() 
{
	float x,y,z;
	
	int count = fscanf(file,"%f %f %f", &x, &y, &z);
	if (count != 3) {
		printf ("Error trying to read 3 floats to make a Vec3f\n");
		assert (0);
	}
	
	return Vec3f(x,y,z);
}

float SceneParser::readFloat() 
{
	float answer;
	
	int count = fscanf(file,"%f",&answer);
	if (count != 1) {
		printf ("Error trying to read 1 float\n");
		assert (0);
	}
	
	return answer;
}

int SceneParser::readInt() 
{
	int answer;
	
	int count = fscanf(file, "%d", &answer);
	if (count != 1) {
		printf ("Error trying to read 1 int\n");
		assert (0);
	}
	
	return answer;
}


// ====================================================================
// ====================================================================

ostream & operator << (ostream & os, const SceneParser & sparser)
{

	os << "Scene Parser Information:" << endl
	   << endl
	   << "Background Color:\t" << sparser.background_color << endl
	   << "Ambient Light Color:\t" << sparser.ambient_light << endl 
	   << endl
	   << sparser.getCamera() << endl;

	os << "Lights Information:" << endl;
	os << "Light Number:\t" << sparser.getLightNum() << endl;

	os << *(sparser.getGroup()) << endl;
	return os;
}
