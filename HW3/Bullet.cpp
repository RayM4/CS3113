#include "Bullet.h"
#define PI 3.14159265359f

Bullet::Bullet(float x, float y, float speed)
	: x(x), y(y), speed(speed) {
	angle = 0.0f;
};

void Bullet::setX(float px) { x = px; }
void Bullet::setY(float py) { y = py; }
void Bullet::setSpeed(float spd) { speed = spd; }
void Bullet::setAngle(float ang) { angle = ang; }
void Bullet::setTime(float time) { timeAlive = time; }

float Bullet::getX() { return x; }
float Bullet::getY() { return y; }
float Bullet::getAngle() { return angle; }
float Bullet::getSpeed() { return speed; }
float Bullet::getTime() { return timeAlive; }

void Bullet::setSprite(const char * image_path, float u, float v, float w, float h){
	SpriteSheet sprite(image_path, u, v, w, h);
	image = sprite;
}

void Bullet::draw(float scale) {
	//maybe visible
	image.draw(scale, x , y);
}

void Bullet::update(float elapsed) {
	x += cos(angle * PI / 180.0f) * elapsed * speed;
	y += sin(angle * PI / 180.0f) * elapsed * speed;
	timeAlive += elapsed;
}