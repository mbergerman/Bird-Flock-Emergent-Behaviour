#pragma once

#include "Tipos.h"

enum {N, EYESIGHT, RANDOMJIGGLE, NUM_KEYS};

typedef struct
{
	int n = 0;
	double eyeSight = 0.0;
	double randomJiggle = 0.0;
	bool keys[3] = { false, false, false };
}uData;

int parseCallback(char* key, char* value, void* userData);
void printCmdLineHelp();
bool checkInputData(uData* userData);