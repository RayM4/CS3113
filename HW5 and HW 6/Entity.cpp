#include "Entity.h"

#define GRAVITY_X 0.0f;
#define GRAVITY_Y -5.0f;
#define FIXED_TIMESTEP 0.0166666f

Entity::Entity(float x, float y, float u, float v, float w, float h, float scale, SpriteSheet* sprites)
	: posX(x), posY(y), u(u), v(v), width(w), height(h), image(sprites), scale(scale) {
	dead = false;
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	friction_x = 0.0f;
	friction_y = 0.0f;
	right = true;
	collideTop = false;
	collideBot = false;
	collideLeft = false;
	collideRight = false;
};

float Entity::getX() { return posX; }
float Entity::getY() { return posY; }
float Entity::getU() { return u; }
float Entity::getV() { return v; }
float Entity::getSpeed() { return speed; }

float Entity::getH() { return height; }
float Entity::getW() { return width; }

float Entity::getVeloX() { return velocity_x; }
float Entity::getVeloY() { return velocity_y; }
float Entity::getAccelX() { return acceleration_x; }
float Entity::getAccelY() { return acceleration_y; }
float Entity::getFricX() { return friction_x; }
float Entity::getFricY() { return friction_y; }
float Entity::getMass() { return mass; }
bool Entity::getStatic() { return isStatic; }
bool Entity::isDead() { return dead; }
SpriteSheet * Entity::getSheet() { return image; }

float Entity::getScale() { return scale;  };

void Entity::setX(float x) { posX = x; }
void Entity::setY(float y) { posY = y; }
void Entity::setU(float x) { u = x; }
void Entity::setV(float y) { v = y; }
void Entity::setH(float h) { height = h; }
void Entity::setW(float w) { width = w; }
void Entity::setSpeed(float spd) { speed = spd; }

void Entity::setVeloX(float x) { velocity_x = x; }
void Entity::setVeloY(float y) { velocity_y = y; }
void Entity::setAccelX(float x) { acceleration_x = x; }
void Entity::setAccelY(float y) { acceleration_y = y; }
void Entity::setFricX(float x) { friction_x = x; }
void Entity::setFricY(float y) { friction_y = y; }
void Entity::setMass(float m) { mass = m; }
void Entity::setStatic(bool s) { isStatic = s; }

void Entity::setLeft() { right = false; }
void Entity::setRight() { right = true; }
bool Entity::getRight() { return right; }

void Entity::setTexture(SpriteSheet* sprite) { image = sprite; }

void Entity::setCTOP(bool a) { collideTop = a; }
void Entity::setCBOT(bool a) { collideBot = a; }
void Entity::setCLEFT(bool a) { collideLeft = a; }
void Entity::setCRIGHT(bool a) { collideRight = a; }

bool Entity::getT() { return collideTop; }
bool Entity::getB() { return collideBot; }
bool Entity::getL() { return collideLeft; }
bool Entity::getR() { return collideRight; }

void Entity::died() { dead = true; }

void Entity::resetFlags() {
	collideTop = false;
	collideBot = false;
	collideLeft = false;
	collideRight = false;
}

void Entity::draw() {
	//if (!dead) {
	//	if (right) {
	//		image->draw(scale, posX, posY, u, v, width, height);
	//	}
	//	else if (!right) {
	//		image->drawf(scale, posX, posY, u, v, width, height);
	//	}

	//}
	if (!dead) {
		image->draw(scale, posX, posY, u, v, width, height);
	}
}

void Entity::draw(float x, float y) {
	image->draw(scale, x, y, u, v, width, height);
}

void Entity::update(float elapsed) {

}

void Entity::fixedUpdate() {
	float a = GRAVITY_Y;
	velocity_y += a * FIXED_TIMESTEP;
	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocity_x += acceleration_x * FIXED_TIMESTEP;
	velocity_y += acceleration_y * FIXED_TIMESTEP;
	posX += velocity_x * FIXED_TIMESTEP;
	posY += velocity_y * FIXED_TIMESTEP;
}

void Entity::notDead() {
	dead = false;
}

float Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

