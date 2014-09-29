//Raymond Mak
//CS3113
//HW 3

#include "SpriteSheet.h"
using namespace std;

SpriteSheet::SpriteSheet() {};

SpriteSheet::SpriteSheet(const char * image_path, float u, float v, float width, float height)
	: textureID(LoadTexture(image_path)), u(u), v(v), width(width), height(height) {} ;

//SpriteSheet::~SpriteSheet() {
//	delete this;
//}

void SpriteSheet::draw(float scale, float x, float y) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);

	GLfloat quad[] = {-width * scale , height * scale, -width * scale, -height * scale, width * scale, -height * scale, width * scale, height * scale};

	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = {u, v, u, v+height, u+width, v+height, u+width, v};

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

float SpriteSheet::getU() { return u; }
float SpriteSheet::getV() { return v; }
float SpriteSheet::getWidth() { return width; }
float SpriteSheet::getHeight() { return height; }
GLuint SpriteSheet::getID() { return textureID; }

void SpriteSheet::setU(float n) { u = n; } 
void SpriteSheet::setV(float n) { v = n; }
void SpriteSheet::setWidth(float n) { width = n; }
void SpriteSheet::setHeight(float n) { height = n; }

GLuint SpriteSheet::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE,surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	SDL_FreeSurface(surface);

	return textureID;
}