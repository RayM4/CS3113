#include "Entity.h"

#define GRAVITY_X 0.0f;
#define GRAVITY_Y -10.0f;
#define FIXED_TIMESTEP 0.0166666f

Entity::Entity(float x, float y, float u, float v, float w, float h, bool isStatic, float mass, SpriteSheet* sprites)
	: posX(x), posY(y), u(u), v(v), width(w), height(h), image(sprites), isStatic(isStatic), mass(mass) {
		dead = false; 
		velocity_x = 0.0f;
		velocity_y = 0.0f;
		acceleration_x = 0.0f;
		acceleration_y = 0.0f;
		friction_x = 0.0f;
		friction_y = 0.0f;
		right = true;
		still = true;
		collideTop = false;
		collideBot = false;
		collideLeft = false;
		collideRight = false;
};
		
float Entity::getX() { return posX; }
float Entity::getY() { return posY; }
float Entity::getU() { return u;  }
float Entity::getV() { return v;  }
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
SpriteSheet * Entity::getSheet() { return image;  }

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

void Entity::setStill(bool a) { still = a; }
bool Entity::getStill() { return still; }

void Entity::setTexture(SpriteSheet* sprite) { image = sprite; }

bool Entity::collidesWith(Entity* entity, float scale1, float scale2) {
	if ((posY - (height * scale1 *0.5f) < entity->getY() + (entity->getH() * scale2*0.5f)) &&
		(posX + (width * scale1 *0.5f) > entity->getX() - (entity->getW() * scale2*0.5f)) &&
		(posX + (width * scale1 *0.5f) < entity->getX() + (entity->getW() * scale2*0.5f))) {
		collideBot = true;
	}
	if ((posY + (height * scale1 *0.5f) > entity->getY() - (entity->getH() * scale2*0.5f)) &&
		(posX + (width * scale1 *0.5f) > entity->getX() - (entity->getW() * scale2*0.5f)) &&
		(posX + (width * scale1 *0.5f) < entity->getX() + (entity->getW() * scale2*0.5f))) {
		collideTop = true;
	}
	if ((posX + (width * scale1 *0.5f) > entity->getX() - (entity->getW() * scale2*0.5f)) &&
		(posY - (height * scale1 *0.5f) < entity->getY() + (entity->getH() * scale2*0.5f)) &&
		(posY + (height * scale1 *0.5f) > entity->getY() - (entity->getH() * scale2*0.5f))) {
		collideRight = true;
	}
	if ((posX - (width * scale1 *0.5f) < entity->getX() + (entity->getW() * scale2*0.5f)) &&
		(posY - (height * scale1 *0.5f) < entity->getY() + (entity->getH() * scale2*0.5f)) &&
		(posY + (height * scale1 *0.5f) > entity->getY() - (entity->getH() * scale2*0.5f))) {
		collideLeft = true;
	}

	/*if ((posY - (height * scale1 * 0.5f) < entity->getY() + (entity->getH() * scale2 * 0.5f)) &&
		(posY + (height * scale1 * 0.5f) > entity->getY() - (entity->getH() * scale2 * 0.5f)) &&
		(posX + (width * scale1 * 0.5f) > entity->getX() - (entity->getW() * scale2 * 0.5f)) &&
		(posX - (width * scale1 * 0.5f) > entity->getX() + (entity->getW() * scale2 * 0.5f))
		) {*/
	if (collideBot || collideTop || collideLeft || collideRight) {
		return true;
	}
	return false;

	//return (collideTop && collideBot && collideLeft && collideRight);
}

void Entity::setCTOP(bool a) { collideTop = a; }
void Entity::setCBOT(bool a) { collideBot = a; }
void Entity::setCLEFT(bool a) { collideLeft = a; }
void Entity::setCRIGHT(bool a) { collideRight = a; }

bool Entity::getT() { return collideTop; }
bool Entity::getB() { return collideBot; }
bool Entity::getL() { return collideLeft; }
bool Entity::getR() { return collideRight; }

void Entity::resetFlags() {
	collideTop = false;
	collideBot = false;
	collideLeft = false;
	collideRight = false;
}

void Entity::yPen(Entity* entity) {
	//float pen = fabs((posY + height * 0.5 * 0.4f) - (entity->getY() - entity->getH() * 0.5 * 0.7f));
	float dist = fabs(entity->getY() - posY);
	float h1 = height *  0.5f * 0.4f;
	float h2 = entity->getH() * 0.5f * 0.7f;
	float pen = fabs(dist - h1 - h2);

	//posY > entity->getY() &&
	//posY < entity->getY() && 
	if (collideBot) {
		posY += pen + 0.001f;
	}
	if (collideTop) {
		posY -= pen + 0.001f;
	}
}

void Entity::xPen(Entity* entity) {
	//float pen = fabs((posx + height * 0.4f) - (entitx->getx() - entitx->getH() * 0.7f));
	float dist = fabs(posX - entity->getX());
	float w1 = width * 0.5f * 0.4f;
	float w2 = entity->getW() * 0.5f * 0.7f;
	float pen = fabs(dist - w1 - w2);

	if (posX < entity->getX() && collideLeft) {
		posY += pen + 0.001f;
	}
	if (posX > entity->getX() && collideTop) {
		posY -= pen + 0.001f;
	}
}

void Entity::draw(float scale) {
	if (!dead) {
		if (right) {
			image->draw(scale, posX, posY, u, v, width, height);
		}
		else if (!right) {
			image->drawf(scale, posX, posY, u, v, width, height);
		}

	}
}

void Entity::update(float elapsed) {

}

void Entity::fixedUpdate() {
	acceleration_y = GRAVITY_Y;
	if (fabs(acceleration_x) < 0.2f) {
		acceleration_x = 0;
	}
	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocity_y = lerp(velocity_y, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocity_x += acceleration_x * FIXED_TIMESTEP;
	velocity_y += acceleration_y * FIXED_TIMESTEP;
	if (acceleration_x != 0) {
		if (acceleration_x > 0) {
			acceleration_x -= FIXED_TIMESTEP * 2.0f;
		} else
			acceleration_x += FIXED_TIMESTEP * 2.0f;
	}
	posX += velocity_x * FIXED_TIMESTEP;
	if (!collideBot) {
		posY += velocity_y * FIXED_TIMESTEP;
	}
}

void Entity::fixedUpdate2() {
	//acceleration_y = GRAVITY_Y;
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

bool Entity::gotHit(std::vector<Bullet>& bullets) {
	if (!dead) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i].getX() > posX - (0.4f * width) && bullets[i].getX() < posX + (0.4f * width)) {
				dead = true;
				bullets[i] = bullets[bullets.size() - 1];
				bullets.pop_back();
				return true;
			}
		}
	}
	
	return false;
}

float Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}