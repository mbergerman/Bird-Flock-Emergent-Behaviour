#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_image.h"
#include "time.h"
#include "Flock.h"
#include "parseCallback.h"
#include "parseCmdLine.h"
#include "UserInterface.h"

const int display_scale = 9;
const int bird_size = 30;

int main(int argc, char* argv[]){
	uData userData;
	pCallback pToCallback = parseCallback;

	srand(time(NULL));

	parseCmdLine(argc, argv, pToCallback, &userData);
	if (checkInputData(&userData) == false) {
		printCmdLineHelp();
	}
	else {
		int error;
		UserInterface UI(100, 70, display_scale, error);
		
		if (error == ERR_NONE) {
			UI.printInstructions();

			Flock flock(userData.n, userData.eyeSight, userData.randomJiggle, 100, 70);

			UI.handleUI(flock, bird_size);
		}
		else {
			printf("Error en la inicializacion de tipo: %d\n", error);
			return error;
		}
	}
}