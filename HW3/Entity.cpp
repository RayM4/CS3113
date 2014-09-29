//Raymond Mak
//CS3113
//HW 3

#include "Entity.h"

//Entity::Entity( float x, float y, float w, float h, const char * image_path) 
//	: posX(x), posY(y), width(w), height(h), textureID(LoadTexture(image_path)) { rotation = 0.0f; };

Entity::Entity( float x, float y, float w, float h, float u, float v, const char * image_path) 
	: posX(x), posY(y), width(w), height(h), image(image_path, u, v, w, h) {
	rotation = 0.0f; dead = false;
};


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

void Entity::draw(float scale) {
	if (!dead){
		image.draw(scale, posX, posY);
	}
}

bool Entity::gotHit(std::vector<Bullet>& bullets) {
	if (!dead) {
		for (int i = 0; i < bullets.size(); i++) {
			if (posY < bullets[i].getY() && (posX + (width / 2.0)) > bullets[i].getX() && (posX - (width / 2.0)) < bullets[i].getX()) {
				dead = true;
				bullets[i] = bullets[bullets.size() - 1];
				bullets.pop_back();
				return true;
			}
		}
	}
	return false;
}

void Entity::gotHit2(std::vector<Bullet> bullets) {
	for (int i = 0; i < bullets.size(); i++) {
		if (posY > bullets[i].getY() && (posX + (width / 2.0)) > bullets[i].getX() && (posX - (width / 2.0)) < bullets[i].getX()) {
			dead = true;
		}
	}
}

bool Entity::isDead() {
	return dead;
}