//Raymond Mak
//CS3113
//HW 4
#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class SpriteSheet {
public:
	SpriteSheet();
	SpriteSheet(const char * image_path);

	void draw(float scale, float x, float y, float u, float v, float w, float h);
	void drawf(float scale, float x, float y, float u, float v, float w, float h);

	GLuint getID();

private:
	GLuint LoadTexture(const char *image_path);

	GLuint textureID;
};