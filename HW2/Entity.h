//Raymond Mak
//CS3113
//HW 2
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity {
public:
	Entity( float x, float y, float w, float h, const char * image_path);
	~Entity();

	float getX();
	float getY();
	float getSpeed();
	float getDirectionX();
	float getDirectionY();

	void setX(float x);
	void setY(float y);
	void setDirX(float x);
	void setDirY(float y);
	void setSpeed(float spd);
	void setRotation(float r);

	void reverseX();
	void reverseY();

	void setTexture(const char * image_path);

	void draw();

private:
	GLuint LoadTexture(const char *image_path);


	float width;
	float height;

	float posX;
	float posY;

	float speed;
	float directionX;
	float directionY;

	float rotation;
	GLuint textureID;
};
