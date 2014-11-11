#include "Entity.h"

#define GRAVITY_X 0.0f;
#define GRAVITY_Y -5.0f;
#define FIXED_TIMESTEP 0.0166666f

Entity::Entity(float x, float y, float u, float v, float w, float h, float scale, float rot,  SpriteSheet* sprites)
	: posX(x), posY(y), u(u), v(v), width(w), height(h), image(sprites), scale(scale), rotation(rot) {
	velocity_x = 0.0f;
	velocity_y = 0.0f;
	acceleration_x = 0.0f;
	acceleration_y = 0.0f;
	rotationV = 0.0f;
	rotationA = 0.0f;
	shift = true;
	cd = 0;
};

float Entity::getX() { return posX; }
float Entity::getY() { return posY; }
float Entity::getU() { return u; }
float Entity::getV() { return v; }
float Entity::getSpeed() { return speed; }

float Entity::getXPen() { return x_pen; }
float Entity::getYPen() { return y_pen; }

float Entity::getH() { return height; }
float Entity::getW() { return width; }

float Entity::getVeloX() { return velocity_x; }
float Entity::getVeloY() { return velocity_y; }
float Entity::getAccelX() { return acceleration_x; }
float Entity::getAccelY() { return acceleration_y; }

float Entity::getRot() { return rotation; }
float Entity::getRotV() { return rotationV; }
float Entity::getRotA() { return rotationA; }

SpriteSheet * Entity::getSheet() { return image; }

float Entity::getScale() { return scale;  };

void Entity::setX(float x) { posX = x; }
void Entity::setY(float y) { posY = y; }
void Entity::setU(float x) { u = x; }
void Entity::setV(float y) { v = y; }
void Entity::setH(float h) { height = h; }
void Entity::setW(float w) { width = w; }
void Entity::setSpeed(float spd) { speed = spd; }

void Entity::setXPen(float x) { x_pen = x; }
void Entity::setYPen(float y) { y_pen = y; }

void Entity::setVeloX(float x) { velocity_x = x; }
void Entity::setVeloY(float y) { velocity_y = y; }
void Entity::setAccelX(float x) { acceleration_x = x; }
void Entity::setAccelY(float y) { acceleration_y = y; }

void Entity::setTexture(SpriteSheet* sprite) { image = sprite; }

void Entity::setRot(float r) { rotation = r; }
void Entity::setRotV(float v) { rotationV = v; }
void Entity::setRotA(float a) { rotationA = a; }

bool Entity::getShift() { return shift; }
void Entity::setShift(bool a) { shift = a; }

void Entity::draw() {
	buildMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.m1);

	image->draw(scale, posX, posY, u, v, width, height);

	glPopMatrix();
}

void Entity::draw(float x, float y) {
	buildMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.m1);
	image->drawf(scale, x, y, u, v, width, height);

	glPopMatrix();
}

void Entity::update(float elapsed) {

}

void Entity::fixedUpdate() {
	//float a = GRAVITY_Y;
	//velocity_y += a * FIXED_TIMESTEP;
	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocity_x += acceleration_x * FIXED_TIMESTEP;
	velocity_y += acceleration_y * FIXED_TIMESTEP;
	rotationV += rotationA * FIXED_TIMESTEP * 6.0f;
	rotationV = lerp(rotationV, 0.0f, FIXED_TIMESTEP * 2.0f);
	rotation += rotationV * FIXED_TIMESTEP;
	posX += (velocity_x * FIXED_TIMESTEP * (cos(rotation + 1.57f))) + (x_pen * FIXED_TIMESTEP * 3.0f);
	posY += (velocity_y * FIXED_TIMESTEP * (sin(rotation + 1.57f))) + (y_pen * FIXED_TIMESTEP * 3.0f);
	x_pen = 0.0f;
	y_pen = 0.0f;
	//rocks bouncing left and right from boundaries fix
	if (!shift) {
		cd++;
		if (cd > 100) {
			cd = 0;
			shift = true;
		}
	} 
}

float Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

void Entity::buildMatrix() {
	Matrix scale;
	scale.matrix[0][0] = SCALE;
	scale.matrix[1][1] = SCALE;

	Matrix rot;
	rot.matrix[0][0] = cos(rotation);
	rot.matrix[1][0] = -sin(rotation);
	rot.matrix[0][1] = sin(rotation);
	rot.matrix[1][1] = cos(rotation);

	Matrix trans;
	trans.matrix[3][0] = posX;
	trans.matrix[3][1] = posY;

	matrix = scale * rot * trans;
}