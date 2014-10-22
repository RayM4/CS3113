//Raymond Mak
//CS3113
//HW 4
#pragma once
#include "Entity.h"
#include "Bullet.h"
#include <string>
#include <vector>
#include <algorithm>
#include <SDL_mixer.h>

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

//#define LEVEL_HEIGHT 20
//#define LEVEL_WIDTH 30
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

	void RenderMainMenu();
	void RenderGameLevel();
	void RenderGameOver(bool win);

	void UpdateMainMenu();
	void UpdateGameLevel();
	void UpdateGameOver(bool win);

	void setWin(bool a);
	void updateScore(int num);

	void resetGame();

	void shootBullet();
	void genBullets();

	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readEntityData(std::ifstream &stream, SpriteSheet* sheet);

	void placeEntity(std::string type, float x, float y);

	void drawTileMap();

	void moveCamera(float x, float y);

	void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY);

	void drawText(const char *image_path, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
	GLuint LoadTexture(const char *image_path);

	float checkPointForGridCollisionX(float x, float y);
	float checkPointForGridCollisionY(float x, float y);
	//bool isSolid(int y, int x);
	bool isSolid(unsigned char tile);
	void doLevelCollisionX(Entity* entity);
	void doLevelCollisionY(Entity* entity);
	bool doBulletCollision(Bullet* bullet);
private:
	bool done;
	bool win;

	int mapWidth;
	int mapHeight;

	int score;
	int cdCount;

	unsigned char** levelData;

	float lastFrameTicks;
	Mix_Music* music;
	Mix_Chunk* gun;

	std::vector<Entity*> entities;
	//std::vector<Entity*> enemies;

	std::vector<Bullet*> bullets;
	SpriteSheet* sheet;
	SDL_Window* displayWindow;
};