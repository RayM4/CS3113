//Raymond Mak
//CS3113
//HW 3
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet {
public:
	SpriteSheet();
	SpriteSheet(const char * image_path, float u, float v, float width, float height);

	void draw(float scale,float x, float y);

	float getU();
	float getV();
	float getWidth();
	float getHeight();
	GLuint getID();

	void setU(float n);
	void setV(float n);
	void setWidth(float n);
	void setHeight(float n);
	
private:
	GLuint LoadTexture(const char *image_path);

	GLuint textureID;
	float u;
	float v;
	float width;
	float height;
};