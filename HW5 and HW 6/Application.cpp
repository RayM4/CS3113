//Raymond Mak
//CS3113
//HW 4

#include "Application.h"
using namespace std;

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define PLAYER 0
#define BULLET_SPEED 1.4f
#define PLAYER_ACCEL 1.0f
#define WORLD_OFFSET_X 0.0f
#define WORLD_OFFSET_Y 0.0f
#define SCALE 0.5f
float timeLeftOver = 0.0f;

SDL_Event event;

const Uint8 *keys = SDL_GetKeyboardState(NULL);
enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };

int state;

Application::Application() {
	mapWidth = 0;
	mapHeight = 0;
	Init();
	done = false;
	lastFrameTicks = 0.0f;
	score = 0;
	cdCount = 0;
	win = false;
	music = Mix_LoadMUS("001.mp3");
	gun = Mix_LoadWAV("002.wav");
}

Application::~Application() {
	Mix_FreeChunk(gun);
	Mix_FreeMusic(music);
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
	glMatrixMode(GL_MODELVIEW);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	sheet = new SpriteSheet("arne_sprites.png");

	string levelFile = "mapdata.txt";
	ifstream infile(levelFile);
	string line;
	bool flag = true;
	while (getline(infile, line) && flag == true) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				flag = false;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			readEntityData(infile, sheet);
		}
	}

	state = STATE_GAME_LEVEL;
	//state = STATE_GAME_OVER; // for Testing
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
			entities[i]->fixedUpdate();
			entities[i]->resetFlags();
			doLevelCollisionY(entities[i]);
			doLevelCollisionX(entities[i]);
		}

		for (int i = 0; i < bullets.size(); i++) {
			bool flag = doBulletCollision(bullets[i]);
			if (flag) {
				bullets[i] = bullets[bullets.size() - 1];
				bullets.pop_back();
				i -= 1;
			}
		}

		for (int i = 0; i < bullets.size(); i++) {
			bullets[i]->update(elapsed);
			if (bullets[i]->getTime() > 40.0f) {
				bullets[i] = bullets[bullets.size() - 1];
				bullets.pop_back();
			}
		}

		if (score == entities.size() - 1) {
			win = true;
			state = STATE_GAME_OVER;
		}

		cdCount++;
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);
	Render();

	return done;
}

void Application::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (state) {
	case STATE_GAME_LEVEL:
		RenderGameLevel();
		break;
	case STATE_GAME_OVER:
		RenderGameOver(win);
		break;
	}

	SDL_GL_SwapWindow(displayWindow);
}


void Application::RenderGameLevel() {
	Mix_PlayMusic(music, -1);
	glLoadIdentity();
	glTranslatef((entities[PLAYER]->getX() * -1.0f), (entities[PLAYER]->getY() * -1.0f), 0.0f);

	drawTileMap();
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->draw();
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->draw(SCALE);
	}
}

void Application::RenderGameOver(bool win) {
	if (win) {
		drawText("font1.png", "You Win!", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, 0.4f);
		drawText("font1.png", "Pree \"R\" ", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, 0.0f);
		drawText("font1.png", "to restart ", 0.4f, -0.25f, 0.6f, 1.0f, 1.0f, 1.0f, -0.7f, -0.2f);
	}
}

void Application::UpdateGameLevel() {
	if (keys[SDL_SCANCODE_LEFT]) {
		entities[PLAYER]->setLeft();
		entities[PLAYER]->setVeloX(-1.0*PLAYER_ACCEL);
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		entities[PLAYER]->setRight();
		entities[PLAYER]->setVeloX(PLAYER_ACCEL);
	}
	else if (keys[SDL_SCANCODE_UP]) {
		entities[PLAYER]->setVeloY(1.3f);
	}
	else if (keys[SDL_SCANCODE_SPACE]) {
		if (cdCount > 10) {
			shootBullet();
			Mix_PlayChannel(-1, gun, 0);
		}
	}
}

void Application::UpdateGameOver(bool win) {
	if (keys[SDL_SCANCODE_R]) {
		if (win) {
			resetGame();
		}
	}
}

void Application::Update(float elapsed) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}
	switch (state) {
	case STATE_GAME_LEVEL:
		UpdateGameLevel();
		break;
	case STATE_GAME_OVER:
		UpdateGameOver(win);
		break;
	}
}

bool Application::readHeader(std::ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {

		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else {
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool Application::readLayerData(std::ifstream &stream) {
	string line;
	while (getline(stream, line)) {

		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "data") {

			for (int y = 0; y < mapHeight; y++) {

				getline(stream, line);
				istringstream lineStream(line);
				string tile;

				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

bool Application::readEntityData(std::ifstream &stream, SpriteSheet* sheet) {
	string line;
	string type;

	while (getline(stream, line)) {
		if (line == "") { break; }

		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);

		if (key == "type") {
			type = value;
		} else if (key == "location") {
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');

			float placeX = (float)(atoi(xPosition.c_str()) / 16 * TILE_SIZE);
			float placeY = (float)(atoi(yPosition.c_str()) / 16 * -TILE_SIZE);
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void Application::placeEntity(string type, float x, float y) {
	if (type == "player") {
		float u = (float)((99 % 16) / 16.0f);
		float v = ((99.0f / 16.0f) / 8.0f);
		float w = 1.0f / 16.0f;
		float h = 1.0f / 8.0f;
		Entity* player = new Entity(x, y, u, v, w, h, SCALE, sheet);
		entities.push_back(player);
	} else if (type == "enemy") {
		float u = (float)((81 % 16) / 16);
		float v = ((81.0f / 16.0f) / 8.0f);
		float w = 1.0f / 16.0f;
		float h = 1.0f / 8.0f;
		Entity* enemy = new Entity(x, y, u, v, w, h, SCALE, sheet);
		entities.push_back(enemy);
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

void Application::drawTileMap() {
	vector<float> vertexData;
	vector<float> texCoordData;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sheet->getID());
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glTranslatef((-TILE_SIZE * mapWidth * 0.1f), (TILE_SIZE * mapHeight * 0.5f), 0.0f);

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (levelData[y][x] != 0) {

				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});
			}
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, vertexData.size() / 2);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();
}

void Application::worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY) {
	*gridX = (int)((worldX + (WORLD_OFFSET_X)) / TILE_SIZE);
	*gridY = (int)((-worldY + (WORLD_OFFSET_Y)) / TILE_SIZE);
}

float Application::checkPointForGridCollisionY(float x, float y){
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 90 || gridY < 0 || gridY > 20){
		return 0.0f;
	}

	if (isSolid(levelData[gridY][gridX])) {
		float yCoordinate = (gridY * TILE_SIZE);
		return -y - yCoordinate;
	}
	return 0.0f;
}

float Application::checkPointForGridCollisionX(float x, float y) {
	int gridX, gridY;
	worldToTileCoordinates(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > 90 || gridY < 0 || gridY > 20){
		return 0.0f;
	}

	if (isSolid(levelData[gridY][gridX])) {
		float xCoordinate = (gridX * TILE_SIZE);
		return -x - xCoordinate;
	}
	return 0.0f;
}

//bool Application::isSolid(int y, int x) {
//	char a = levelData[y][x];
//	//int num = a - '0';
//	if (a != '0') {
//		return true;
//	}
//	else {
//		return false;
//	}
//}

bool Application::isSolid(unsigned char tile){
	switch (tile){
	case 16:
	case 17:
	case 18:
	case 19:
	case 33:
	case 34:
	case 35:
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

void Application::doLevelCollisionY(Entity* entity){
	float adjust = checkPointForGridCollisionY(entity->getX(), entity->getY() - entity->getH()*0.5f);
	if (adjust != 0.0f){
		entity->setY(entity->getY() + adjust + 0.001f);
		entity->setVeloY(0.0f);
		entity->setCBOT(true);
	}

	adjust = checkPointForGridCollisionY(entity->getX(), entity->getY() + entity->getH()*0.5);
	if (adjust != 0.0f){
		entity->setY(entity->getY() - adjust - 0.0001f);
		entity->setVeloY(0.0f);
		entity->setCTOP(true);
	}
}

void Application::doLevelCollisionX(Entity* entity){
	float adjust = checkPointForGridCollisionX(entity->getX() - entity->getW() * 0.5, entity->getY());
	if (adjust != 0.0f) {
		entity->setX(entity->getX() - (adjust * TILE_SIZE) + 0.001f);
		entity->setVeloX(0.0f);
		entity->setCLEFT(true);
	}

	adjust = checkPointForGridCollisionX(entity->getX() + entity->getW()*0.5, entity->getY());
	if (adjust != 0.0f) {
		entity->setX(entity->getX() + (adjust * TILE_SIZE) - 0.001f);
		entity->setVeloX(0.0f);
		entity->setCRIGHT(true);
	}
}

void Application::shootBullet() {
	float u = (float)((50 % 16) / 16);
	float v = ((50.0f / 16.0f) / 8.0f);
	float w = 1.0f / 16.0f;
	float h = 1.0f / 8.0f;
	Bullet *newBullet = new Bullet(entities[PLAYER]->getX(), entities[PLAYER]->getY(), u,v,w,h, BULLET_SPEED, sheet);
	if (entities[PLAYER]->getRight()) {
		newBullet->setX(newBullet->getX() + 0.1f);
		newBullet->setAngle(0.0f);
	}
	else {
		newBullet->setX(newBullet->getX() - 0.1f);
		newBullet->setAngle(180.0f);
	}
	bullets.push_back(newBullet);
	cdCount = 0;

}

bool Application::doBulletCollision(Bullet* bullet) {
	int gridX, gridY;
	worldToTileCoordinates(bullet->getX(), bullet->getY(), &gridX, &gridY);
	for (int i = 1; i < entities.size(); i++) {
		int grid2, grid3;
		worldToTileCoordinates(entities[i]->getX(), entities[i]->getY(), &grid2, &grid3);
		if (gridX >(grid2 - entities[i]->getW()) && gridX < (grid2 + entities[i]->getW()) && (!entities[i]->isDead())) {
			entities[i]->died();
			score++;
			return true;
		}
	}
	return false;
}

void Application::resetGame() {
	glLoadIdentity();

	for (int i = 0; i < entities.size(); i++) {
		delete entities[i];
		entities[i] = nullptr;
	}
	for (int i = 0; i < bullets.size(); i++) {
		delete bullets[i];
		bullets[i] = nullptr;
	}
	entities.clear();
	bullets.clear();

	mapWidth = 0;
	mapHeight = 0;
	done = false;
	lastFrameTicks = 0.0f;
	score = 0;
	cdCount = 0;
	win = false;

	glMatrixMode(GL_MODELVIEW);

	string levelFile = "mapdata.txt";
	ifstream infile(levelFile);
	string line;
	bool flag = true;
	while (getline(infile, line) && flag == true) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				flag = false;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			readEntityData(infile, sheet);
		}
	}

	state = STATE_GAME_LEVEL;
}