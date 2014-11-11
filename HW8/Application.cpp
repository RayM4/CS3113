//Raymond Mak
//CS3113
//HW 4

#include "Application.h"
using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define PLAYER 0
#define PLAYER_ACCEL 1.0f
#define SCALE 1.0f
#define NUM_ROCKS 10
#define SHAKE 50
float timeLeftOver = 0.0f;

SDL_Event event;

const Uint8 *keys = SDL_GetKeyboardState(NULL);
enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };

int state;

Application::Application() {
	Init();
	lastFrameTicks = 0.0f;
	done = false;
	screenShakeValue = 0.0f;
	screenShakeSpeed = -5.0f;
	screenShakeIntensity = 0.005f;
	ssCount = SHAKE + 1;
	fade = 0.0f;
	fadeT = 0.0f;
	//music = Mix_LoadMUS("001.mp3");
}

Application::~Application() {
	//Mix_FreeMusic(music);
	IMG_Quit();
	SDL_Quit();
}

void Application::Init() {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_MODELVIEW);
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	sheet = new SpriteSheet("sheet1.png");

	float u = 112.0f/1024.0f;
	float v = 791.0f/1024.0f;
	float w = 112.0f/1024.0f;
	float h = 75.0f / 1024.0f;
	Entity* player = new Entity(0.0f, 0.0f, u, v, w, h, SCALE, 90.0f, sheet);
	entities.push_back(player);

	srand(time(NULL));
	for (int i = 0; i < NUM_ROCKS; i++) {
		int id = rand() % 8;
		float x = (rand() % 18 -9) * 0.1f;
		float y = (rand() % 18 -9) * 0.1f;
		float u = 0.0f;
		float v = 0.0f;
		float w = 0.0f;
		float h = 0.0f;
		//randomize asteriods
		switch (id) {
		case 0:
			u = 224.0f / 1024.0f;
			v = 664.0f / 1024.0f;
			w = 101.0f / 1024.0f;
			h = 84.0f / 1024.0f;
			break;
		case 1:
			u = 0.0f / 1024.0f;
			v = 520.0f / 1024.0f;
			w = 120.0f / 1024.0f;
			h = 98.0f / 1024.0f;
			break;
		case 2:
			u = 224.0f / 1024.0f;
			v = 664.0f / 1024.0f;
			w = 101.0f / 1024.0f;
			h = 84.0f / 1024.0f;
			break;
		case 3:
			u = 651.0f / 1024.0f;
			v = 447.0f / 1024.0f;
			w = 43.0f / 1024.0f;
			h = 43.0f / 1024.0f;
			break;
		case 4:
			u = 224.0f / 1024.0f;
			v = 748.0f / 1024.0f;
			w = 101.0f / 1024.0f;
			h = 84.0f / 1024.0f;
			break;
		case 5:
			u = 327.0f / 1024.0f;
			v = 452.0f / 1024.0f;
			w = 98.0f / 1024.0f;
			h = 96.0f / 1024.0f;
			break;
		case 6:
			u = 237.0f / 1024.0f;
			v = 452.0f / 1024.0f;
			w = 45.0f / 1024.0f;
			h = 40.0f / 1024.0f;
			break;
		case 7:
			u = 512.0f / 1024.0f;
			v = 728.0f / 1024.0f;
			w = 89.0f / 1024.0f;
			h = 82.0f / 1024.0f;
			break;
		default:
			u = 396.0f / 1024.0f;
			v = 413.0f / 1024.0f;
			w = 29.0f / 1024.0f;
			h = 26.0f / 1024.0f;
			break;
		}
		float randRot = (rand() % 9 - 4) * 90.0f ;
		Entity* Rock = new Entity(x, y, u, v, w, h, SCALE, randRot, sheet);
		entities.push_back(Rock);
	}

	state = STATE_GAME_LEVEL;
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

		for (int i = 0; i < entities.size(); i++){
			if (i > 0) {
				entities[i]->setVeloX(0.25f + entities[i]->getXPen());
				entities[i]->setVeloY(0.25f + entities[i]->getYPen());
			}

			entities[i]->fixedUpdate();
			
			if (fabs(entities[i]->getX()) > 0.999f && entities[i]->getShift()) {
				entities[i]->setX(entities[i]->getX() * -1.0f);
				entities[i]->setShift(false);
			}
			if (fabs(entities[i]->getY()) > 0.999f && entities[i]->getShift()) {
				entities[i]->setY(entities[i]->getY() * -1.0f);
				entities[i]->setShift(false);
			}
			
		}
		//update particles
		for (int i = 0; i < particles.size(); i++) {
			if (particles[i]->getTimer() > 1000) {
				delete particles[i];
				particles[i] = nullptr;
				particles.erase(particles.begin() + i);
				i--;
			}
			else {
				particles[i]->Update(elapsed);
			}
		}

		//collision
		for (int i = 0; i < entities.size(); i++){
			for (int j = 0; j < entities.size(); j++) {
				if (checkCollision(entities[i],entities[j])) {
					if (i == 0) {
						shipToRock();
						ssCount = 0;
					}
					else {
						rockToRock(entities[i]);
					}
					collisionResp(entities[i],entities[j]);
				}
			}
		}

	}
	timeLeftOver = fixedElapsed;
	fade = easeIn(1.0f, 0.0f, fadeT);
	if (fadeT < 100.0f) {
		fadeT += elapsed * 0.5f;
	}
	Update(elapsed);
	Render();
	if (ssCount < SHAKE) {
		screenShake(elapsed);
		if (ssCount == SHAKE) {
			ssCount = SHAKE + 1;
		}
		ssCount++;
	}
	return done;
}

void Application::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	RenderGameLevel();

	SDL_GL_SwapWindow(displayWindow);
}


void Application::RenderGameLevel() {
	//Mix_PlayMusic(music, -1);
	

	int a = 0;
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->draw();
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i]->Render();
	}

	if (fadeT < 1.0f) {
		Fade(fade);
	}

}

void Application::UpdateGameLevel() {
	if (keys[SDL_SCANCODE_LEFT]) {
		entities[PLAYER]->setRotV(5.0f);
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		entities[PLAYER]->setRotV(-5.0f);
	}
	else if (keys[SDL_SCANCODE_UP]) {
		entities[PLAYER]->setVeloX(0.8f);
		entities[PLAYER]->setVeloY(0.8f);
		//shipMove();
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		entities[PLAYER]->setVeloX(-0.8f);
		entities[PLAYER]->setVeloY(-0.8f);
	}
}

void Application::Update(float elapsed) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	//switch (state) {
	//case STATE_GAME_LEVEL:
	//	UpdateGameLevel();
	//	break;
	//}
	UpdateGameLevel();
}

bool Application::checkCollision(Entity* e1, Entity* e2) {
	if (e1 == e2 || e1 == nullptr || e2 == nullptr){
		return false;
	}
	e1->buildMatrix();
	e2->buildMatrix();

	Matrix entity1Inverse = e1->matrix.inverse();
	Matrix entity2Inverse = e2->matrix.inverse();

	//creating vectors for each vertex
	Vector e2TL = Vector(-e2->getW() * SCALE * 0.5f, e2->getH() * SCALE * 0.5f, 0.0);
	Vector e2BL = Vector(-e2->getW() * SCALE * 0.5f, -e2->getH() * SCALE * 0.5f, 0.0);
	Vector e2TR = Vector(e2->getW() * SCALE * 0.5f, e2->getH() * SCALE * 0.5f, 0.0);
	Vector e2BR = Vector(e2->getW() * SCALE * 0.5f, -e2->getH() * SCALE * 0.5f, 0.0);

	//to world coord
	e2TL = e2->matrix * e2TL;
	e2BL = e2->matrix * e2BL;
	e2TR = e2->matrix * e2TR;
	e2BR = e2->matrix * e2BR;

	e2TL = entity1Inverse * e2TL;
	e2BL = entity1Inverse * e2BL;
	e2TR = entity1Inverse * e2TR;
	e2BR = entity1Inverse * e2BR;

	//axis check
	float minX = std::min(std::min(std::min(e2TL.x, e2BL.x), e2TR.x), e2BR.x);
	float maxX = std::max(std::max(std::max(e2TL.x, e2BL.x), e2TR.x), e2BR.x);

	if (!(minX <= e1->getW()*0.5 && maxX >= -e1->getW()*0.5)){
		return false;
	}

	float minY = std::min(std::min(std::min(e2TL.y, e2BL.y), e2TR.y), e2BR.y);
	float maxY = std::max(std::max(std::max(e2TL.y, e2BL.y), e2TR.y), e2BR.y);

	if (!(minY <= e1->getH()*0.5 && maxY >= -e1->getH()*0.5)){
		return false;
	}

	return true;
}

void Application::collisionResp(Entity* e1, Entity* e2) {
	//e1->buildMatrix();
	//e2->buildMatrix();

	//Matrix entity1Inverse = e1->matrix.inverse();
	//Matrix entity2Inverse = e2->matrix.inverse();

	//creating vectors for each vertex
	Vector e1TL = Vector(-e1->getW() * SCALE * 0.5f, e1->getH() * SCALE * 0.5f, 0.0);
	Vector e1BL = Vector(-e1->getW() * SCALE * 0.5f, -e1->getH() * SCALE * 0.5f, 0.0);
	Vector e1TR = Vector(e1->getW() * SCALE * 0.5f, e1->getH() * SCALE * 0.5f, 0.0);
	Vector e1BR = Vector(e1->getW() * SCALE * 0.5f, -e1->getH() * SCALE * 0.5f, 0.0);

	Vector e2TL = Vector(-e2->getW() * SCALE * 0.5f, e2->getH() * SCALE * 0.5f, 0.0);
	Vector e2BL = Vector(-e2->getW() * SCALE * 0.5f, -e2->getH() * SCALE * 0.5f, 0.0);
	Vector e2TR = Vector(e2->getW() * SCALE * 0.5f, e2->getH() * SCALE * 0.5f, 0.0);
	Vector e2BR = Vector(e2->getW() * SCALE * 0.5f, -e2->getH() * SCALE * 0.5f, 0.0);

	//to world coord
	e1TL = e1->matrix * e1TL;
	e1BL = e1->matrix * e1BL;
	e1TR = e1->matrix * e1TR;
	e1BR = e1->matrix * e1BR;

	e2TL = e2->matrix * e2TL;
	e2BL = e2->matrix * e2BL;
	e2TR = e2->matrix * e2TR;
	e2BR = e2->matrix * e2BR;

	float e1_x = ((e1TL.x + e1BL.x + e1TR.x + e1BR.x) * 0.25f);
	float e2_x = ((e2TL.x + e2BL.x + e2TR.x + e2BR.x) * 0.25f);

	float pen_x = fabs(e1_x - e2_x);

	float e1_y = ((e1TL.y + e1BL.y + e1TR.y + e1BR.y) * 0.25f);
	float e2_y = ((e2TL.y + e2BL.y + e2TR.y + e2BR.y) * 0.25f);

	float pen_y = fabs(e1_y - e2_y);

	float distance = sqrt(pen_x * pen_x + pen_y * pen_y);

	//float x_pen1 = 0.0f;
	//float y_pen1 = 0.0f;
	//float x_pen2 = 0.0f;
	//float y_pen2 = 0.0f;

	//axis pen 
	if (e1_x > e2_x) {
		e1->setXPen(pen_x / distance);
		e2->setXPen((-1 * pen_x) / distance);
	}
	else {
		e2->setXPen(pen_x / distance);
		e1->setXPen((-1 * pen_x) / distance);
	}

	if (e1_y > e2_y) {
		e1->setYPen(pen_y / distance);
		e2->setYPen((-1 * pen_y) / distance);
	}
	else {
		e2->setYPen(pen_y / distance);
		e1->setYPen((-1 * pen_y) / distance);
	}
}

float Application::lerp(float from, float to, float time) {
	return (1.0 - time)*from + time*to;
}

void Application::rockToRock(Entity* e1) {
	ParticleEmitter* sparks = new ParticleEmitter(60, e1->getX(), e1->getY(), 0.0f, -2.0f, 0.0f, 0.0f, 1.2f, 1.2f);
	sparks->setSColor(1.0f, 0.58f, 0.0f, 1.0f);
	sparks->setEColor(1.0f, 0.0f, 0.0f, 1.0f);
	particles.push_back(sparks);
}

void Application::shipToRock() {
	ParticleEmitter* boom = new ParticleEmitter(300, entities[PLAYER]->getX(), entities[PLAYER]->getY(), 0.0f, -2.0f, 0.0f, 0.7f, 0.7f, 1.0f);
	boom->setSColor(1.0f, 1.0f, 0.0f, 1.0f);
	boom->setEColor(0.0f, 0.15f, 1.0f, 1.0f);
	particles.push_back(boom);
}

void Application::shipMove() {
	ParticleEmitter* fire = new ParticleEmitter(50, entities[PLAYER]->getX(), entities[PLAYER]->getY(), 0.0f, -2.0f, entities[PLAYER]->getVeloX() * -1.0f, entities[PLAYER]->getVeloY() * -1.0f, 0.7f, 1.0f);
	fire->setSColor(0.0f, 1.0f, 0.06f, 1.0f);
	fire->setEColor(0.0f, 1.0f, 0.48f, 1.0f);
	particles.push_back(fire);
}

void Application::screenShake(float elapsed) {
	screenShakeValue += elapsed;
	glTranslatef(cos(screenShakeValue * screenShakeSpeed)* screenShakeIntensity, 0.0f, 0.0f);
}

void Application::Fade(float a) {
	glMatrixMode(GL_MODELVIEW);
	Matrix mat;
	glPushMatrix();

	glMultMatrixf(mat.m1);
	GLfloat midLine2[] = { -1.33f, -1.0f, 1.33f, -1.0f, 1.33f, 1.0f, -1.33f, 1.0f };
	glVertexPointer(2, GL_FLOAT, 0, midLine2);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat midColor2[] = { 0.0f, 0.0f, 0.0f, a, 0.0f, 0.0f, 0.0f, a, 0.0f, 0.0f, 0.0f, a, 0.0f, 0.0f, 0.0f, a };
	glColorPointer(4, GL_FLOAT, 0, midColor2);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glDisableClientState(GL_COLOR_ARRAY);
}

float Application::easeIn(float from, float to, float time) {
	float tVal = time*time*time*time*time;
	return (1.0f - tVal)*from + tVal*to;
}