#include "Bullet.h"
#define PI 3.14159265359f

Bullet::Bullet(float x, float y, float u, float v, float w, float h, float speed, SpriteSheet* sprite)
	: x(x), y(y), u(u), v(v), w(w), h(h), speed(speed), image(sprite) {
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

void Bullet::draw(float scale) {
	//maybe visible
	image->draw(scale, x, y, u, v, w, h);
}

void Bullet::update(float elapsed) {
	x += cos(angle * PI / 180.0f) * elapsed * speed;
	y += sin(angle * PI / 180.0f) * elapsed * speed;
	timeAlive += elapsed;
}