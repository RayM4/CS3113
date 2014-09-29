//Raymond Mak
//CS3113
//HW 3

#include "Application.h"
using namespace std;

int main(int argc, char *argv[]) {

	Application app;
	while(!app.UpdateAndRender()) {}
	return 0;
}