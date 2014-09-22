//Raymond Mak
//CS3113
//HW 2

//#include <sdl.h>
//#include <sdl_opengl.h>
//#include <sdl_image.h>
#include "Entity.h"

SDL_Window* displayWindow;
const Uint8 *keys = SDL_GetKeyboardState(NULL);
SDL_Event event;

void setup() {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_MODELVIEW);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.4f, 0.4f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void processEvent(Entity** arr, float* elapsed, int* score1, int* score2) {

	//Ball movement
	if (arr[4]->getX() > 0.85 || arr[4]->getX() < -0.85 ) {
		arr[4]->setTexture("ball_v.png");
		arr[4]->setX(0.0f);
		arr[4]->setY(0.0f);
		arr[4]->setDirX(-0.5f);
		arr[4]->setDirY(0.2f);
	}

	arr[4]->setX( arr[4]->getX() + (*elapsed * arr[4]->getDirectionX() * arr[4]->getSpeed() ));
	arr[4]->setY( arr[4]->getY() + (*elapsed * arr[4]->getDirectionY() * arr[4]->getSpeed() ));
			
	if( arr[4]->getY() > 0.8 || arr[4]->getY() < -0.8 ) {
		arr[4]->reverseY();
	}
	if ( (arr[4]->getX() < arr[2]->getX() + 0.2f) && (arr[4]->getY() < (arr[2]->getY() + 0.2f)) && (arr[4]->getY() > (arr[2]->getY() - 0.2f)) && arr[4]->getX() < 0.0f) {
			arr[4]->reverseX(); 
	}
	if ( (arr[4]->getX() > arr[3]->getX() - 0.2f) && (arr[4]->getY() < (arr[3]->getY() + 0.2f)) && (arr[4]->getY() > (arr[3]->getY() - 0.2f)) && arr[4]->getX() > 0.0f ) {
		arr[4]->reverseX();
	}

	if (arr[4]->getX() > 0.85 ) {
		arr[4]->setTexture("explode.png");
		*score1 += 1;
	} else if (arr[4]->getX() < -0.85) {
		arr[4]->setTexture("explode.png");
		*score2 += 1;
	}
			
	//Player Input

	if(keys[SDL_SCANCODE_DOWN] && (arr[3]->getY() - (*elapsed * arr[3]->getSpeed()) > -0.8f)) {
		arr[3]->setY( arr[3]->getY() - (*elapsed * arr[3]->getSpeed()) );
	} else if(keys[SDL_SCANCODE_UP] && ((arr[3]->getY() + (*elapsed * arr[3]->getSpeed())) < 0.8f)) {
		arr[3]->setY( (arr[3]->getY() + (*elapsed * arr[3]->getSpeed())) );
	} else if(keys[SDL_SCANCODE_W] && ((arr[2]->getY() + (*elapsed * arr[2]->getSpeed())) < 0.8f)) {
		arr[2]->setY( (arr[2]->getY() + (*elapsed * arr[2]->getSpeed())) );
	} else if(keys[SDL_SCANCODE_S] && (arr[2]->getY() - (*elapsed * arr[2]->getSpeed()) > -0.8f)) {
		arr[2]->setY( arr[2]->getY() - (*elapsed * arr[2]->getSpeed()) );
	}
	
}

void render(Entity** arr, int size) {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < size; i++) {
		arr[i]->draw();
	}
}

void quit() {
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char *argv[]) {
	setup();

	//const Uint8 *keys = SDL_GetKeyboardState(NULL);
	//SDL_Event event;

	bool done = true;

	float lastFrameTicks = 0.0f;
	float *elapsed = new float(0.0f);
	int* score1 = new int(0);
	int* score2 = new int(0);
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	*elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	//creating entities
	Entity *topWall = new Entity(0.0f,1.0f,2.66f,0.2f, "wall.png");
	Entity *botWall = new Entity(0.0f,-1.0f,2.66f,0.2f, "wall.png");
	Entity *left = new Entity(-0.85f,0.0f,0.3f,0.4f, "left_m.png");
	Entity *right = new Entity(0.85f,0.0f,0.3f,0.4f, "right_k.png");
	Entity *ball = new Entity(0.0f,0.0f,0.1f,0.1f, "ball_v.png");
	ball->setDirX(-0.5f);
	ball->setDirY(0.2f);
	ball->setSpeed(0.4f);
	left->setSpeed(0.6f);
	right->setSpeed(0.6f);

	Entity *win = new Entity(0.3f,0.0f, 0.3f,0.3f, "win.png");

	Entity* enti[5] = { topWall, botWall, left, right, ball };

	while (done) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = false; 
			} 

			processEvent(enti, elapsed, score1,score2);
		}
		render(enti, 5);

		if (*score1 > 3) {
			done = false;
			glClear(GL_COLOR_BUFFER_BIT);
			left->setX(0.0f);
			left->setY(0.0f);
			left->draw();
			win->draw();
		} else if (*score2 > 3) {
			done = false;
			glClear(GL_COLOR_BUFFER_BIT);
			right->setX(0.0f);
			right->setY(0.0f);
			right->draw();
			win->draw();
		}

		SDL_GL_SwapWindow(displayWindow);
	}

	quit();
	return 0;
};