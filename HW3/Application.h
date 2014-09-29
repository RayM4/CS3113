//Raymond Mak
//CS3113
//HW 3
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
	void DrawText(const char *image_path, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);

	void shootBullet();
	void genBullets();
	bool shouldRemoveBullet(Bullet bullet);

	void moveE(std::vector<Entity>& enemy);

private:
	GLuint LoadTexture(const char *image_path);
	bool done;
	bool win;
	bool toggle;
	int score;
	int cdCount;
	int cdCount2;
	float lastFrameTicks;
	std::vector<Entity> entites;
	std::vector<Entity> enemiesR1;
	std::vector<Entity> enemiesR2;
	std::vector<Entity> enemiesR3;
	std::vector<Entity> enemiesR4;
	std::vector<Bullet> bullets;
	std::vector<Bullet> bullets2;
	SDL_Window* displayWindow;
};