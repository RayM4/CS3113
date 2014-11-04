//Raymond Mak
//CS3113
//HW 4
#pragma once

#define FIXED_TIMESTEP 0.0166666f
#define SCALE 1.0f
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SpriteSheet.h"
#include <vector>

#include "Matrix.h"


class Entity {
public:
	Entity(float x, float y, float u, float v, float w, float h, float scale, float rot, SpriteSheet* sprites);

	float getX();
	float getY();
	float getU();
	float getV();
	float getSpeed();
	float getH();
	float getW();
	float getRot();
	float getRotV();
	float getRotA();

	float getVeloX();
	float getVeloY();
	float getAccelX();
	float getAccelY();

	float getScale();

	float getXPen();
	float getYPen();

	SpriteSheet * getSheet();

	void setX(float x);
	void setY(float y);
	void setU(float x);
	void setV(float y);
	void setH(float h);
	void setW(float w);
	void setSpeed(float spd);
	void setXPen(float x);
	void setYPen(float y);

	void setVeloX(float x);
	void setVeloY(float y);
	void setAccelX(float x);
	void setAccelY(float y);
	void setRot(float r);
	void setRotV(float v);
	void setRotA(float a);

	void setTexture(SpriteSheet* sprites);

	bool getShift();
	void setShift(bool a);

	void update(float elapsed);

	void fixedUpdate();
	void fixedUpdate2();

	void yPen(Entity* entity);
	void xPen(Entity* entity);

	void draw();
	void draw(float x, float y);

	void buildMatrix();

	Matrix matrix;
private:
	float lerp(float v0, float v1, float t);

	float width;
	float height;
	float u;
	float v;

	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;

	float posX;
	float posY;

	float x_pen;
	float y_pen;

	float speed;
	float scale; 

	float rotation;
	float rotationV;
	float rotationA;

	bool shift;
	int cd;

	SpriteSheet* image;
};