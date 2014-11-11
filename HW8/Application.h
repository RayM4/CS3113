//Raymond Mak
//CS3113
//HW 4
#pragma once
#include "Entity.h"
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <SDL_mixer.h>
#include "ParticleEmitter.h"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8
#define TILE_SIZE 0.09f


class Application {
public:
	Application();
	~Application();

	void Init();
	bool UpdateAndRender();
	void Render();
	void Update(float elapsed);

	void RenderGameLevel();
	void UpdateGameLevel();

	bool checkCollision(Entity* e1, Entity* e2);
	void collisionResp(Entity* e1, Entity* e2);

	float calDistance(Entity* e1, Entity* e2, float* e1_x, float* e2_x, float* e1_y, float* e2_y);

	bool isSolid(Entity* entity);

	void rockToRock(Entity* e1);
	void shipToRock();
	void shipMove();
	void screenShake(float elapsed);

	void Fade(float a);

	float screenShakeValue;
	float screenShakeSpeed;
	float screenShakeIntensity;

	float easeIn(float from, float to, float time);
private:
	bool done;
	int ssCount;
	float fade;
	float fadeT;

	unsigned char** levelData;

	float lastFrameTicks;

	float lerp(float from, float to, float time);

	//Mix_Music* music;

	std::vector<Entity*> entities;
	std::vector<ParticleEmitter*> particles;
	std::vector<ParticleEmitter*> particles2;

	SpriteSheet* sheet;
	SDL_Window* displayWindow;
};