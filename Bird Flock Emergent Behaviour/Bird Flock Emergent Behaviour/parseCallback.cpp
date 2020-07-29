#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseCallback.h"

int parseCallback(char* key, char* value, void* userData) {
	int ret = 0;
	if (key == NULL) {
		if (strcmp(value, "help")) {
			printCmdLineHelp();
			ret = 1;
		}
		else {
			//No hay parametros en este TP
		}
	}
	else {
		if (strcmp(key, "n") == 0) {
			((uData*)userData)->n = atoi(value);
			((uData*)userData)->keys[N] = true;
			ret = 1;
		}else if (strcmp(key, "eyeSight") == 0) {
			((uData*)userData)->eyeSight = atof(value);
			((uData*)userData)->keys[EYESIGHT] = true;
			ret = 1;
		}else if (strcmp(key, "randomJiggle") == 0) {
			((uData*)userData)->randomJiggle = atof(value);
			((uData*)userData)->keys[RANDOMJIGGLE] = true;
			ret = 1;
		}
	}
	return ret;
}

void printCmdLineHelp() {
	printf("Se deben indicar los parametros 'n' , 'eyeSight' y 'randomJiggle'\n");
	printf("Ejemplo: -n 13 -eyeSight 20 -randomJiggle 12.5\n");
}

bool checkInputData(uData* userData) {
	bool ret = true;
	for (int i = 0; i < NUM_KEYS; i++) {
		if (userData->keys[i] == false) {
			ret = false;
		}
	}
	return ret;
}