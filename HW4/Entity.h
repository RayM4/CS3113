//Raymond Mak
//CS3113
//HW 4
#pragma once

#define FIXED_TIMESTEP 0.0166666f

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SpriteSheet.h"
#include "Bullet.h"
#include <vector>

class Entity {
public:
	Entity(float x, float y, float u, float v, float w, float h, bool isStatic, float mass, SpriteSheet* sprites);

	float getX();
	float getY();
	float getU();
	float getV();
	float getSpeed();
	float getH();
	float getW();

	float getVeloX();
	float getVeloY();
	float getAccelX();
	float getAccelY();
	float getFricX();
	float getFricY();
	float getMass();
	bool getStatic();
	
	bool isDead();

	SpriteSheet * getSheet();

	void setX(float x);
	void setY(float y);
	void setU(float x);
	void setV(float y);
	void setH(float h);
	void setW(float w);
	void setSpeed(float spd);

	void setVeloX(float x);
	void setVeloY(float y);
	void setAccelX(float x);
	void setAccelY(float y);
	void setFricX(float x);
	void setFricY(float y);
	void setMass(float m);
	void setStatic(bool s);

	bool getT();
	bool getB();
	bool getL();
	bool getR();

	void setCTOP(bool a);
	void setCBOT(bool a);
	void setCLEFT(bool a);
	void setCRIGHT(bool a);

	void resetFlags();

	void setRight();
	void setLeft();

	void setTexture(SpriteSheet* sprites);

	void update(float elapsed);
	bool collidesWith(Entity* entity, float scale1, float scale2);

	bool collideX(Entity* entity, float scale1, float scale2);
	bool collideY(Entity* entity, float scale1, float scale2);

	void fixedUpdate();
	void fixedUpdate2();

	void setStill(bool a);
	bool getStill();

	void yPen(Entity* entity);
	void xPen(Entity* entity);

	bool getRight();

	bool gotHit(std::vector<Bullet>& bullets);
	void draw(float scale);
	void notDead();
private:
	float lerp(float v0, float v1, float t);

	bool dead;

	float width;
	float height;
	float u;
	float v;

	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float friction_x;
	float friction_y;
	float mass;
	bool isStatic;

	bool enableCollisions;

	bool collideTop;
	bool collideBot;
	bool collideLeft;
	bool collideRight;

	bool right;
	bool still;

	float posX;
	float posY;

	float speed;

	SpriteSheet* image;
};