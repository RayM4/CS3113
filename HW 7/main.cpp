//Raymond Mak
//CS3113
//HW 4

#include "Application.h"
using namespace std;

int main(int argc, char *argv[]) {

	Application app;
	while (!app.UpdateAndRender()) {}
	return 0;
}