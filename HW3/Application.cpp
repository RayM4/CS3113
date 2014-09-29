//Raymond Mak
//CS3113
//HW 3

#include "Application.h"
using namespace std;

#define NUM_ENEMY_ROW 10
#define ENEMY_SPACE 0.2f
#define PLAYER 2
#define PLAYER_SPEED 0.02f
#define BULLET_SPEED 1.4f
#define MAX_BULLET 30
#define COOLDOWN 20

SDL_Event event;

const Uint8 *keys = SDL_GetKeyboardState(NULL);
enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};

int state;

Application::Application() {
	Init();
	done = false;
	toggle = true;
	lastFrameTicks = 0.0f;
	score = 0;
}
Application::~Application() {
	IMG_Quit();
	SDL_Quit();
	delete this;
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
	glMatrixMode(GL_MODELVIEW);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Entity charizard(-0.5f, 0.0f, 156.0f / 512.0f, 130.0f / 512.0f, 0.0f, 0.0f, "sprites.png");
	Entity venasaur(0.5f, 0.0f, 153.0f / 512.0f, 124.0f / 512.0f, 0.0f, 132.0f / 512.0f, "sprites.png");
	Entity dritini(0.0f, -0.8f, 132.0f / 512.0f, 129.0f / 512.0f, 139.0f / 512.0f, 258.0f / 512.0f, "sprites.png");
	Entity beedrill(-0.9f, 0.8f, 110.0f / 512.0f, 133.0f / 512.0f, 273.0f / 512.0f, 132.0f / 512.0f, "sprites.png");
	Entity gliscor(-0.9f, 0.6f, 117.0f / 512.0f, 121.0f / 512.0f, 0.0f, 386.0f / 512.0f, "sprites.png");
	Entity magikarp(0.0f, -0.3f, 117.0f/512.0f, 130.0f/512.0f, 158.0f/512.0f,0.0f, "sprites.png");
	Entity pikachu(0.0f, -0.3f, 137.0f/512.0f,126.0f/512.0f,0.0f, 258.0f/512.0f, "sprites.png");

	entites.push_back(charizard);
	entites.push_back(venasaur);
	entites.push_back(dritini);
	entites.push_back(beedrill);
	entites.push_back(gliscor);
	entites.push_back(magikarp);
	entites.push_back(pikachu);

	Entity beedrill2(-0.9f, 0.4f, 110.0f / 512.0f, 133.0f / 512.0f, 273.0f / 512.0f, 132.0f / 512.0f, "sprites.png");
	Entity gliscor2(-0.9f, 0.2f, 117.0f / 512.0f, 121.0f / 512.0f, 0.0f, 386.0f / 512.0f, "sprites.png");

	float xe = ENEMY_SPACE;
	enemiesR1.push_back(beedrill);
	enemiesR2.push_back(gliscor);
	enemiesR3.push_back(beedrill2);
	enemiesR4.push_back(gliscor2);
	for (int i = 0; i < NUM_ENEMY_ROW; i++) {
		beedrill.setX(beedrill.getX() + xe);
		beedrill2.setX(beedrill2.getX() + xe);
		enemiesR1.push_back(beedrill);
		enemiesR3.push_back(beedrill2);

		gliscor.setX(gliscor.getX() + xe);
		gliscor2.setX(gliscor2.getX() + xe);
		enemiesR2.push_back(gliscor);
		enemiesR4.push_back(gliscor2);
	}


	cdCount = 10;
	cdCount2 = 10;
	state = STATE_MAIN_MENU;
}

bool Application::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks()/1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
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
	entites[0].draw(1.0);
	entites[1].draw(1.0);

	DrawText("font1.png", "PokeShoota", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, 0.4f);
	DrawText("font1.png", "Press Enter", 0.2f, -0.1f, 1.0f, 0.4f, 0.6f, 1.0f, -0.7f, -0.4f);
	DrawText("font1.png", "To Start", 0.2f, -0.1f, 1.0f, 0.4f, 0.6f, 1.0f, -0.5f, -0.6f);

	
}

void Application::RenderGameLevel() {
	entites[PLAYER].draw(0.35f);
	DrawText("font1.png", to_string((score * 10)), 0.2f, -0.1f, 0.6f, 1.0f, 1.0f, 1.0f, 0.6f, -0.6f);
	
	for (int i = 0; i < NUM_ENEMY_ROW; i++) {
		enemiesR1[i].draw(0.3f);
		enemiesR2[i].draw(0.3f);
		enemiesR3[i].draw(0.3f);
		enemiesR4[i].draw(0.3f);
	}

	for (int i = 0; i < bullets.size(); i++) {
			bullets[i].draw(0.15f);
	}
	for (int i = 0; i < bullets2.size(); i++) {
			bullets2[i].draw(0.15f);
	}
}

void Application::RenderGameOver(bool win) {
	if (win) {
		DrawText("font1.png", "You Win", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, 0.4f);
		entites[6].draw(0.8);
		/*string sc = to_string((score * 10));
		sc = "score" + sc;*/
		string sc = "Score: ";
		sc.append(to_string((score * 10)));
		DrawText("font1.png", sc, 0.2f, -0.1f, 0.6f, 1.0f, 1.0f, 1.0f, -0.2f, -0.6f);
	}
	else {
		DrawText("font1.png", "You Lose", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, 0.4f);
		entites[5].draw(0.8);
		string sc = "Score: ";
		sc.append(to_string((score * 10)));
		DrawText("font1.png", sc, 0.2f, -0.1f, 0.6f, 1.0f, 1.0f, 1.0f, -0.2f, -0.6f);
	}
}

void Application::Update(float elapsed) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true; 
		}
	}

	cdCount++;
	cdCount2++;

	//bullets.erase(std::remove_if(bullets.begin(), bullets.end(), shouldRemoveBullet), bullets.end());
	for (int i = 0; i < bullets2.size(); i++) {
		if (bullets2[i].getY() < -0.9f ) {
			bullets2[i] = bullets2[bullets2.size() - 1];
			bullets2.pop_back();
		}
		else {
			bullets2[i].update(elapsed);
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].getY() > 0.9f) {
			bullets[i] = bullets[bullets.size() - 1];
			bullets.pop_back();
		}
		else {
			bullets[i].update(elapsed);
		}
	}

	for (int i = 0; i < NUM_ENEMY_ROW; i++) {
		if (enemiesR1[i].gotHit(bullets)) {
			score++;
		}
		if (enemiesR2[i].gotHit(bullets)) {
			score++;
		}
		if (enemiesR3[i].gotHit(bullets)) {
			score++;
		}
		if (enemiesR4[i].gotHit(bullets)) {
			score++;
		}
	}

	moveE(enemiesR1);
	moveE(enemiesR2);
	moveE(enemiesR3);
	moveE(enemiesR4);

	if (toggle == true) {
		toggle = false;
	}
	else {
		toggle = true;
	}

	entites[PLAYER].gotHit2(bullets2);
	if (entites[PLAYER].isDead()) {
		win = false;
		state = STATE_GAME_OVER;
	}

	if (score == NUM_ENEMY_ROW * 4) {
		win = true;
		state = STATE_GAME_OVER;
	}

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

void Application::UpdateMainMenu() {
	if (keys[SDL_SCANCODE_KP_ENTER] || keys[SDL_SCANCODE_P]) {
		state = STATE_GAME_LEVEL;
	}
}

void Application::UpdateGameLevel() {
	if (cdCount2 > COOLDOWN) {
		genBullets();
	}

	if (keys[SDL_SCANCODE_LEFT] && entites[PLAYER].getX() - PLAYER_SPEED > -1.0) {
		entites[PLAYER].setX(entites[PLAYER].getX() - PLAYER_SPEED);
	}
	else if (keys[SDL_SCANCODE_RIGHT] && entites[PLAYER].getX() + PLAYER_SPEED < 1.0) {
		entites[PLAYER].setX(entites[PLAYER].getX() + PLAYER_SPEED);
	}
	else if (keys[SDL_SCANCODE_SPACE]) {
		if (cdCount > COOLDOWN) {
			shootBullet();
		}
	}
	
}

void Application::UpdateGameOver(bool win) {
	if (keys[SDL_SCANCODE_R]) {
		done = true;
	}
}

void Application::DrawText(const char *image_path, string text, float size, float spacing, float r, float g, float b, float a, float x, float y) {
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

void Application::shootBullet() {
	Bullet newBullet(entites[PLAYER].getX(), entites[PLAYER].getY(), BULLET_SPEED);
	newBullet.setAngle(90.0f);
	newBullet.setSprite("attacks.png", 18.0f / 64.0f, 27.0f / 64.0f, 12.0f / 64.0f, 28.0f / 64.0f);
	bullets.push_back(newBullet);
	cdCount = 0;
}

void Application::genBullets() {
	int idx = rand() % (NUM_ENEMY_ROW);
	int iy = rand() % 4;
	vector<vector<Entity>> temp;
	temp.insert(temp.end(), { enemiesR1, enemiesR2, enemiesR3, enemiesR4 });
	if (!temp[iy][idx].isDead()) {
		Bullet newBullet(temp[iy][idx].getX(), temp[iy][idx].getY(), BULLET_SPEED);
		newBullet.setAngle(270.0f);
		newBullet.setSprite("attacks.png", 44.0f / 64.0f, 0.0f / 64.0f, 10.0f / 64.0f, 45.0f / 64.0f);
		bullets2.push_back(newBullet);
		cdCount2 = 7;
	}
}

void Application::moveE(vector<Entity>& enemy) {
	if (toggle) {
		for (int i = 0; i < enemy.size(); i++) {
			float dis = (float)(rand() % 3)*0.01f;
			if (enemy[i].getX() + dis < 1.0) {
				enemy[i].setX(enemy[i].getX() + dis);
			}
		}
	}
	else if (!toggle) {
		for (int i = 0; i < enemy.size(); i++) {
			float dis = (float)(rand() % 3)*0.01f;
			if (enemy[i].getX() - dis > -1.0) {
				enemy[i].setX(enemy[i].getX() - dis);
			}
		}
	}
}

//bool Application::shouldRemoveBullet(Bullet bullet) {
//	if (bullet.getTime() > 0.4) {
//		return true;
//	}
//	else { return false; }
//}