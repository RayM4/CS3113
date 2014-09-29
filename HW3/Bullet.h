#pragma once
#include "SpriteSheet.h"
#include <math.h>

class Bullet {
public:
	Bullet(float x, float y, float speed);

	void setX(float x);
	void setY(float y);
	void setSpeed(float spd);
	void setAngle(float angle);
	void setTime(float time);
	void setSprite(const char * image_path, float u, float v, float w, float h);

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
	float angle;
	float speed;
	float timeAlive;
	SpriteSheet image;
};