//Raymond Mak
//CS3113
//HW 4

#include "Application.h"
using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define PLAYER 0
#define PLAYER_ACCEL 2.0f
#define PLAYER_JUMP 2.0f
#define MAX_ENEMY_ON_SCREEN 20
#define MAX_ENEMY 40
#define BULLET_SPEED 1.4f
#define COOLDOWN 20
float timeLeftOver = 0.0f;

SDL_Event event;

const Uint8 *keys = SDL_GetKeyboardState(NULL);
enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };

int state;

Application::Application() {
	Init();
	done = false;
	lastFrameTicks = 0.0f;
	score = 0;
	cdCount = 0;
	toggle = false;
	//keys = SDL_GetKeyboardState(NULL);
}

Application::~Application() {
	IMG_Quit();
	SDL_Quit();
}

void Application::setWin(bool a) {
	win = a;
}


void Application::updateScore(int num) {
	score = num;
}

void Application::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	SpriteSheet* sheet = new SpriteSheet("sprites.png");

	Entity* megaman = new Entity(0.0f, -0.5f, 41.0f / 128.0f, 116.0f / 256.0f, 34.0f / 128.0f, 41.0f / 256.0f, false, 1.0f, sheet);
	Entity* met = new Entity(-0.2f, -0.5f, 45.0f / 128.0f, 75.0f / 256.0f, 25.0f / 128.0f, 25.0f / 256.0f, false, 1.0f, sheet);
	Entity* wall_h = new Entity(-0.4f, -0.8f, 0.0f, 0.0f, 114.0f / 128.0f, 31.0f / 256.0f, true, 2.0f, sheet);
	Entity* wall_v = new Entity(-0.95f, 0.5f, 77.0f / 128.0f, 33.0f / 256.0f, 20.0f / 128.0f, 125.0f / 256.0f, true, 2.0, sheet);
	Entity* wall_h2 = new Entity(0.4f, -0.8f, 0.0f, 0.0f, 114.0f / 128.0f, 31.0f / 256.0f, true, 2.0f, sheet);
	Entity* wall_v2 = new Entity(-0.95f, -0.1f, 77.0f / 128.0f, 33.0f / 256.0f, 20.0f / 128.0f, 125.0f / 256.0f, true, 2.0, sheet);

	Entity* wall_v3 = new Entity(0.95f, 0.5f, 77.0f / 128.0f, 33.0f / 256.0f, 20.0f / 128.0f, 125.0f / 256.0f, true, 2.0, sheet);
	Entity* wall_v4 = new Entity(0.95f, -0.1f, 77.0f / 128.0f, 33.0f / 256.0f, 20.0f / 128.0f, 125.0f / 256.0f, true, 2.0, sheet);
	Entity* wall_v5 = new Entity(-0.95f, -0.4f, 77.0f / 128.0f, 33.0f / 256.0f, 20.0f / 128.0f, 125.0f / 256.0f, true, 2.0, sheet);
	Entity* wall_v6 = new Entity(0.95f, -0.4f, 77.0f / 128.0f, 33.0f / 256.0f, 20.0f / 128.0f, 125.0f / 256.0f, true, 2.0, sheet);

	Entity* wall_h3 = new Entity(-0.9f, 0.0f, 0.0f, 0.0f, 114.0f / 128.0f, 31.0f / 256.0f, true, 2.0f, sheet);
	Entity* wall_h4 = new Entity(0.9f, -0.4f, 0.0f, 0.0f, 114.0f / 128.0f, 31.0f / 256.0f, true, 2.0f, sheet);
	Entity* wall_h5 = new Entity(0.0f, 0.55f, 0.0f, 0.0f, 114.0f / 128.0f, 31.0f / 256.0f, true, 2.0f, sheet);

	entities.push_back(megaman);
	entities.push_back(wall_h);
	entities.push_back(wall_v);
	entities.push_back(wall_h2);
	entities.push_back(wall_v2);
	entities.push_back(wall_v3);
	entities.push_back(wall_v4);
	entities.push_back(wall_v5);
	entities.push_back(wall_v6);
	entities.push_back(wall_h3);
	entities.push_back(wall_h4);
	entities.push_back(wall_h5);

	for (int i = 0; i < MAX_ENEMY_ON_SCREEN; i++) {
		Entity* met2 = new Entity(-0.2f + (i * 0.2f), -0.7f, 45.0f / 128.0f, 75.0f / 256.0f, 25.0f / 128.0f, 25.0f / 256.0f, false, 1.0f, sheet);
		enemies.push_back(met2);
	}
	state = STATE_MAIN_MENU;
	//state = STATE_GAME_LEVEL;
}

bool Application::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		entities[PLAYER]->fixedUpdate();
		for (int i = 0; i < MAX_ENEMY_ON_SCREEN; i++) {
			enemies[i]->fixedUpdate2();
		}
		fixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();

	return done;
}

void Application::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state) {
	case STATE_MAIN_MENU:
		RenderMainMenu();
		break;
	case STATE_GAME_LEVEL:
		RenderGameLevel();
		break;
	case STATE_GAME_OVER:
		RenderGameOver(win);
		break;
	}

	SDL_GL_SwapWindow(displayWindow);
}


void Application::RenderMainMenu() {
	entities[PLAYER]->setY(0.0f);
	entities[PLAYER]->draw(0.5f);
	entities[PLAYER]->setY(-0.65f);

	drawText("font1.png", "MegaMan I", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, 0.4f);
	drawText("font1.png", "Press \"p\"", 0.2f, -0.1f, 1.0f, 0.4f, 0.6f, 1.0f, -0.7f, -0.4f);
	drawText("font1.png", "To Start", 0.2f, -0.1f, 1.0f, 0.4f, 0.6f, 1.0f, -0.5f, -0.6f);
}

void Application::RenderGameLevel() {
	glClear(GL_COLOR_BUFFER_BIT);

	entities[PLAYER]->draw(0.4f);

	for (int i = 1; i < entities.size(); i++) {
		entities[i]->draw(0.7f);
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].draw(0.5f);
	}

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->draw(0.4f);
	}

	

	//drawText("font1.png", to_string((score * 10)), 0.2f, -0.1f, 0.6f, 1.0f, 1.0f, 1.0f, 0.6f, -0.6f);
}

void Application::RenderGameOver(bool win) {
	drawText("font1.png", "You Win!", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, 0.4f);
	drawText("font1.png", "Pree \"R\" ", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, 0.0f);
	drawText("font1.png", "to restart ", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, -0.2f);
}

void Application::UpdateMainMenu() {
	if (keys[SDL_SCANCODE_KP_ENTER] || keys[SDL_SCANCODE_P]) {
		state = STATE_GAME_LEVEL;
	}
}

void Application::UpdateGameLevel() {
	if (keys[SDL_SCANCODE_LEFT]) {
		entities[PLAYER]->setLeft();
		entities[PLAYER]->setStill(false);
		entities[PLAYER]->setAccelX(-1.0*PLAYER_ACCEL);
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		entities[PLAYER]->setRight();
		entities[PLAYER]->setStill(false);
		entities[PLAYER]->setAccelX(PLAYER_ACCEL);
	}
	else if (keys[SDL_SCANCODE_UP]) {
		if (entities[PLAYER]->getB()) {
			entities[PLAYER]->setCBOT(false);
			entities[PLAYER]->setStill(false);
			//entities[PLAYER]->setY(entities[PLAYER]->getY() + 0.1f);
			entities[PLAYER]->setVeloY(PLAYER_JUMP);
		}
	}

	if (keys[SDL_SCANCODE_SPACE]) {
		if (cdCount > 200) {
			shootBullet();
			toggle = true;
		}
	}
	else {
		toggle = false;
	}

	if (score == MAX_ENEMY_ON_SCREEN) {
		state = STATE_GAME_OVER;
	}
}

void Application::UpdateGameOver(bool win) {
	if (keys[SDL_SCANCODE_R]) {
		score = 0;
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->notDead();
		}
		entities[PLAYER]->setX(0.0f);
		entities[PLAYER]->setY(-0.5f);
		state = STATE_GAME_LEVEL;
	}
}

void Application::drawText(const char *image_path, string text, float size, float spacing, float r, float g, float b, float a, float x, float y) {
	GLuint fontTexture(LoadTexture(image_path));
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);

	float texture_size = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
			(-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
			* size });

		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

GLuint Application::LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}

void Application::Update(float elapsed) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}

	/*if (!toggle) {
		entities[PLAYER]->setU(41.0f / 128.0f);
		entities[PLAYER]->setV(116.0f / 256.0f);
		entities[PLAYER]->setW(34.0f / 128.0f);
		entities[PLAYER]->setH(41.0f / 256.0f);
	}
	else {
		entities[PLAYER]->setU(0.0f / 128.0f);
		entities[PLAYER]->setV(116.0f / 256.0f);
		entities[PLAYER]->setW(39.0f / 128.0f);
		entities[PLAYER]->setH(41.0f / 256.0f);
	}*/

	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->gotHit(bullets)) {
			score++;
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i].update(elapsed);
		if (bullets[i].getX() > 0.95f) {
			bullets[i] = bullets[bullets.size() - 1];
			bullets.pop_back();
		}
		else if (bullets[i].getX() < -0.95f) {
			bullets[i] = bullets[bullets.size() - 1];
			bullets.pop_back();
		}
	}

	

	moveE();
	//entities[PLAYER]->resetFlags();
	
	//for (int i = 1; i < entities.size(); i++) {
	//	entities[PLAYER]->collidesWith(entities[i], 0.4f, 0.7f);
	//	if (entities[PLAYER]->getB() || entities[PLAYER]->getT()) {
	//		entities[PLAYER]->yPen(entities[i]);
	//		entities[PLAYER]->setVeloY(0.0f);
	//	}
	//	if (entities[PLAYER]->getL() || entities[PLAYER]->getR()) {
	//		entities[PLAYER]->xPen(entities[i]);
	//	}
	//	//if (entities[PLAYER]->collidesWith(entities[i], 0.4f, 0.7f)) {
	//	//	entities[PLAYER]->yPen(entities[i]);
	//	//	entities[PLAYER]->setVeloY(0.0f);
	//		//if (entities[PLAYER]->getL() || entities[PLAYER]->getR()) {
	//		//	entities[i]->xPen(entities[PLAYER]);
	//		//	entities[PLAYER]->setVeloX(0.0f);
	//		//}
	//		//entities[PLAYER]->setCBOT(true);
	//	//}
	//}

	if (entities[PLAYER]->getX() > 0.95f) {
		entities[PLAYER]->setX(0.80f);
		entities[PLAYER]->setVeloX(0.0f);
		entities[PLAYER]->setAccelX(0.0f);
	}

	if (entities[PLAYER]->getX() < -0.95f) {
		entities[PLAYER]->setX(-0.80f);
		entities[PLAYER]->setVeloX(0.0f);
		entities[PLAYER]->setAccelX(0.0f);
	}

	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i]->getX() > 0.95f) {
			enemies[i]->setX(0.80);
			enemies[i]->setVeloX(0.0f);
			enemies[i]->setAccelX(enemies[i]->getAccelX() * -1.0f);
		}
		else if (enemies[i]->getX() < -0.95f) {
			enemies[i]->setX(-0.80);
			enemies[i]->setVeloX(0.0f);
			enemies[i]->setAccelX(enemies[i]->getAccelX() * -1.0f);
		}
	}

	cdCount++;
	cdCount2++;

	switch (state) {
	case STATE_MAIN_MENU:
		UpdateMainMenu();
		break;
	case STATE_GAME_LEVEL:
		UpdateGameLevel();
		break;
	case STATE_GAME_OVER:
		UpdateGameOver(win);
		break; 
	}

}

void Application::shootBullet() {
	Bullet newBullet(entities[PLAYER]->getX(), entities[PLAYER]->getY(), 
		45.0f/128.0f, 102.0f/256.0f, 8.0f/128.0f, 6.0f/256.0f, BULLET_SPEED, entities[PLAYER]->getSheet());
	if (entities[PLAYER]->getRight()) {
		newBullet.setX(newBullet.getX() + 0.1f);
		newBullet.setAngle(0.0f);
	}
	else {
		newBullet.setX(newBullet.getX() - 0.1f);
		newBullet.setAngle(180.0f);
	}
	bullets.push_back(newBullet);
	cdCount = 0;

}

void Application::fixedUpdate() {
	entities[PLAYER]->resetFlags();

	for (int i = 1; i < entities.size(); i++) {
		bool cond = entities[PLAYER]->collidesWith(entities[i], 0.4f, 0.7f);
		if (entities[PLAYER]->getT() || entities[PLAYER]->getB()) {
			//entities[PLAYER]->yPen(entities[i]);
			//entities[PLAYER]->setVeloY(0.0f);
		}
		//if (cond && (entities[PLAYER]->getL() || entities[PLAYER]->getR())) {
		//	//entities[PLAYER]->xPen(entities[i]);
		//	//entities[PLAYER]->setVeloX(0.0f);
		//	
		//}
		//if (entities[PLAYER]->collidesWith(entities[i], 0.4f, 0.7f)) {
		//	entities[PLAYER]->yPen(entities[i]);
		//	entities[PLAYER]->setVeloY(0.0f);
		//if (entities[PLAYER]->getL() || entities[PLAYER]->getR()) {
		//	entities[i]->xPen(entities[PLAYER]);
		//	entities[PLAYER]->setVeloX(0.0f);
		//}
		//entities[PLAYER]->setCBOT(true);
		//}
	}

	if (!toggle) {
		entities[PLAYER]->setU(41.0f / 128.0f);
		entities[PLAYER]->setV(116.0f / 256.0f);
		entities[PLAYER]->setW(34.0f / 128.0f);
		entities[PLAYER]->setH(41.0f / 256.0f);
	}
	else {
		entities[PLAYER]->setU(0.0f / 128.0f);
		entities[PLAYER]->setV(116.0f / 256.0f);
		entities[PLAYER]->setW(39.0f / 128.0f);
		entities[PLAYER]->setH(41.0f / 256.0f);
	}

}

void Application::moveE() {
	if (cdCount2 > 70) {
		for (int i = 0; i < enemies.size(); i++) {
			int ran = rand() % 2;
			float one = -1.0;
			if (ran == 0) {
				one = 1.0;
			}
			enemies[i]->setAccelX(one * (rand() % 8));

		}
		cdCount2 = 0;
	}
}