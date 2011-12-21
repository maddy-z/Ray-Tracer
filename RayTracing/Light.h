#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Vector.h"

#include <cmath>
#include <cfloat>
#include <string>
#include <iostream>
using namespace std;

// ====================================================================
// ====================================================================

class Light 

{

public:

	// CONSTRUCTOR & DESTRUCTOR
	Light(Vec3f c = Vec3f(1.0f, 1.0f, 1.0f)) : color(c) {}
	virtual ~Light() {}

	// VIRTUAL METHOD  
	virtual float getDistance(const Vec3f & pt) const = 0;
	virtual void getIllumination (const Vec3f & p, Vec3f & dir, Vec3f & col) const = 0;
	virtual std::string toString() = 0;

	void setColor(const Vec3f &c) { color = c; }
	Vec3f getColor() const { return color; }

protected:

	Vec3f color;

};

// ====================================================================
// ====================================================================

class DirectionalLight : public Light 
{

public:

	// CONSTRUCTOR & DESTRUCTOR
	DirectionalLight(Vec3f c = Vec3f(1.0f, 1.0f, 1.0f)) : Light(c) { 
		direction = Vec3f(0.0f, 0.0f, 0.0f);
	}

	DirectionalLight(const Vec3f & d, const Vec3f & c) : Light(c) {
		direction = d; direction.Normalize();
	}

	virtual ~DirectionalLight() {}

	// VIRTUAL METHOD
	float getDistance(const Vec3f & pt) const 
	{
		return FLT_MAX;
	}

	void getIllumination (const Vec3f &p, Vec3f &dir, Vec3f &col) const 
	{
		// the direction to the light is the opposite of the
		// direction of the directional light source.
	
		dir = direction * (-1.0f); dir.Normalize();
		col = color; 
	}

	virtual std::string toString()
	{
		return string("DirectionalLight:\n")
			 + string("\tDirection = ") + direction.toString() + NEW_LINE_CHAR
			 + string("\tColor = ") + color.toString() + NEW_LINE_CHAR;
	}

private:

	// REPRESENTATION
	Vec3f direction;

};


class PointLight : public Light
{

public:

	PointLight(const Vec3f & pos, const Vec3f & c, const Vec3f & att = Vec3f(1.0f, 0.0f, 0.0f)) : Light(c), position(pos), attenuation(att) {} 
	virtual ~PointLight() {}

	virtual float getDistance(const Vec3f & point) const 
	{
		Vec3f d = point - position;
		return sqrt(d[0]*d[0] + d[1]*d[1] + d[2]*d[2]);
	}

	virtual void getIllumination (const Vec3f & p, Vec3f & dir, Vec3f & col) const
	{
		dir = position - p;
		float dist = dir.Length();
		float att = 1.0f / (attenuation[0] + attenuation[1]*dist + attenuation[2]*dist*dist);
		
		dir.Normalize();
		col = color * att;
	}

	Vec3f getPosition() const { return position; }
	Vec3f getAttenuation() const { return attenuation; }

	void setPosition(const Vec3f & pos) { position = pos; }
	void setAttenuation(const Vec3f & att) { attenuation = att; } 

	virtual std::string toString()
	{
		return string("PointLight:\n")
			 + string("\tPosition = ") + position.toString() + NEW_LINE_CHAR
			 + string("\tColor = ") + color.toString() + NEW_LINE_CHAR
			 + string("\tAttenuation = ") + attenuation.toString() + NEW_LINE_CHAR;
	}
	
private:

	Vec3f position;
	Vec3f attenuation;

};

// ==============================================================
// ==============================================================

#endif
