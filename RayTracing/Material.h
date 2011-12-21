#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "Vector.h"

class Ray;
class Hit;
class SceneParser;

// ========================================
// ========================================

class Material
{

public:

	Material(const Vec3f & diffColor,
			 const Vec3f & transColor,
			 const Vec3f & refColor,
			 float iRefraction) : diffuseColor(diffColor), 
								  transparentColor(transColor),
								  reflectiveColor(refColor),
								  indexOfRefraction(iRefraction)
	{}

	virtual ~Material() {}

	virtual Vec3f shade(const Ray & ray, 
						const Hit & hit, 
						const Vec3f & dirToLight, 
						const Vec3f & lightColor) const = 0;
	
	Vec3f getDiffuseColor() const { return diffuseColor; }
	Vec3f getTransparentColor() const { return transparentColor; }
	Vec3f getReflectiveColor() const { return reflectiveColor; }
	float getIndexOfRefraction() const { return indexOfRefraction; }

	virtual std::string toString()
	{
		char str[15];
		sprintf(str, "%f", indexOfRefraction);

		return string("Material:\n")
			 + string("\tDiffuse Color = ") + diffuseColor.toString() + NEW_LINE_CHAR
			 + string("\tTransparent Color = ") + transparentColor.toString() + NEW_LINE_CHAR
			 + string("\tReflective Color = ") + reflectiveColor.toString() + NEW_LINE_CHAR
			 + string("\tIndex Of Refraction = ") + string(str) + NEW_LINE_CHAR;
	}

	bool isMirror() const { return ( reflectiveColor[0] > 0.0f || reflectiveColor[1] > 0.0f || reflectiveColor[2] > 0.0f ); }
	bool isTransparent() const { return ( transparentColor[0] > 0.0f || transparentColor[1] > 0.0f || transparentColor[2] > 0.0f ); }


protected:

	Vec3f diffuseColor;
	Vec3f transparentColor;
	Vec3f reflectiveColor;

	float indexOfRefraction;

private:

};

// ==================================
// ==================================

class PhongMaterial : public Material
{
	friend class SceneParser;

public:
	
	PhongMaterial() : Material(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), 0.0f),
					  specularColor(Vec3f(0.0f, 0.0f, 0.0f)),
					  exponent(0.0f)
	{}

	PhongMaterial(const Vec3f & diffColor,
				  const Vec3f & specColor, float exp,
				  const Vec3f & transColor,
				  const Vec3f & refColor,
				  float iRef) :  Material(diffColor, transColor, refColor, iRef),
								 specularColor(specColor),
								 exponent(exp)
	{}

	virtual ~PhongMaterial() {}

	virtual Vec3f shade(const Ray & ray, 
						const Hit & hit, 
						const Vec3f & dirToLight, 
						const Vec3f & lightColor) const;

	Vec3f getSpecularColor() const { return specularColor; }
	float getExponent() const { return exponent; }

	virtual std::string toString()
	{
		char str1[15], str2[15];
		sprintf(str1, "%f", exponent);
		sprintf(str2, "%f", indexOfRefraction);

		return string("PhongMaterial:\n")
			 + string("\tDiffuse Color = ") + diffuseColor.toString() + NEW_LINE_CHAR
			 + string("\tSpecular Color = ") + specularColor.toString() + NEW_LINE_CHAR
			 + string("\tSpecular Exp = ") + string(str1) + NEW_LINE_CHAR
			 + string("\tTransparent Color = ") + transparentColor.toString() + NEW_LINE_CHAR
			 + string("\tReflective Color = ") + reflectiveColor.toString() + NEW_LINE_CHAR
			 + string("\tIndex Of Refraction = ") + string(str2) + NEW_LINE_CHAR;
	}

protected:

	Vec3f specularColor;
	float exponent;

};

#endif