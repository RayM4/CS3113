#pragma once
#include "SpriteSheet.h"
#include <math.h>

class Bullet {
public:
	Bullet(float x, float y, float u, float v, float w, float h, float speed, SpriteSheet* sprite);

	void setX(float x);
	void setY(float y);
	void setSpeed(float spd);
	void setAngle(float angle);
	void setTime(float time);

	float getX();
	float getY();
	float getAngle();
	float getSpeed();
	float getTime();

	void draw(float scale);
	void update(float elapsed);
private:
	float x;
	float y;
	float u;
	float v;
	float h;
	float w;
	float angle;
	float speed;
	float timeAlive;
	SpriteSheet* image;
};