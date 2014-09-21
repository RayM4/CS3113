//Raymond Mak
//CS3113
//HW 2

#include "Entity.h"

Entity::Entity( float x, float y, float w, float h, const char * image_path) 
	: posX(x), posY(y), width(w), height(h), textureID(LoadTexture(image_path)) { rotation = 0.0f; };
Entity::~Entity() {
	delete this;
}

float Entity::getX() { return posX; }
float Entity::getY() { return posY; }
float Entity::getSpeed() { return speed; }
float Entity::getDirectionX() { return directionX; }
float Entity::getDirectionY() { return directionY; }

void Entity::setX(float x) { posX = x; }
void Entity::setY(float y) { posY = y; }
void Entity::setDirX(float x) { directionX = x; }
void Entity::setDirY(float y) { directionY = y; }
void Entity::setSpeed(float spd) { speed = spd; }
void Entity::setRotation(float r) { rotation = r; }

void Entity::reverseX() { directionX = directionX * (-1.0f); } 
void Entity::reverseY() { directionY = directionY * (-1.0f); } 

void Entity::setTexture(const char * image_path) { textureID = LoadTexture(image_path); } 

void Entity::draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(posX, posY, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	float w1 = width*(-0.5f);
	float w2 = width*(0.5f);
	float h1 = height*(0.5f);
	float h2 = height*(-0.5f);

	GLfloat quad[] = {w1, h1, w1, h2, w2, h2, w2, h1} ;
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

GLuint Entity::LoadTexture(const char *image_path) {
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
