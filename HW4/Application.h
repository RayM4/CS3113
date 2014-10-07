//Raymond Mak
//CS3113
//HW 4
#pragma once
#include "Entity.h"
#include "Bullet.h"
#include <string>
#include <vector>
#include <algorithm>

class Application {
public:
	Application();
	~Application();

	void Init();
	bool UpdateAndRender();
	void Render();
	void Update(float elapsed);

	void RenderMainMenu();
	void RenderGameLevel();
	void RenderGameOver(bool win);

	void UpdateMainMenu();
	void UpdateGameLevel();
	void UpdateGameOver(bool win);

	void setWin(bool a);
	void updateScore(int num);
	void drawText(const char *image_path, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
	GLuint LoadTexture(const char *image_path);

	void shootBullet();
	void genBullets();

	void fixedUpdate();

	void moveE();
private:
	//const Uint8 *keys;
	bool done;
	bool win;

	int score;
	int cdCount;
	int cdCount2;
	bool toggle;

	float lastFrameTicks;

	std::vector<Entity*> entities;
	std::vector<Entity*> enemies;
	std::vector<Bullet> bullets;
	SDL_Window* displayWindow;
};