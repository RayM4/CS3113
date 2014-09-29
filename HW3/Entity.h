//Raymond Mak
//CS3113
//HW 3
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "SpriteSheet.h"
#include "Bullet.h"
#include <vector>

class Entity {
public:
	//Entity( float x, float y, float w, float h, const char * image_path);
	Entity( float x, float y, float w, float h, float u, float v, const char * image_path);
	//~Entity();

	float getX();
	float getY();
	float getSpeed();
	float getDirectionX();
	float getDirectionY();
	bool isDead();

	void setX(float x);
	void setY(float y);
	void setDirX(float x);
	void setDirY(float y);
	void setSpeed(float spd);
	void setRotation(float r);

	void reverseX();
	void reverseY();

	bool gotHit(std::vector<Bullet>& bullets);
	void gotHit2(std::vector<Bullet> bullets);

	//void setTexture(const char * image_path);

	void draw(float scale);

private:
	//GLuint LoadTexture(const char *image_path);

	bool dead;

	float width;
	float height;

	float posX;
	float posY;

	float speed;
	float directionX;
	float directionY;

	float rotation;
	//GLuint textureID;
	SpriteSheet image;
};
