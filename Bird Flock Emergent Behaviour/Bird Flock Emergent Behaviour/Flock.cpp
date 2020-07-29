#include <stdlib.h>
#include <new>
#include "Tipos.h"
#include "Flock.h"
#include "Bird.h"

Flock::Flock(uint n, double e, double j, uint w, uint h) {
	size = n;
	eyeSight = e;
	randomJiggle = j;
	width = w;
	height = h;
	mode = MODE1;

	birds = new (std::nothrow) Bird[n];

	for (uint i = 0; i < n; i++) {
		birds[i].setRandomPos(width, height);
		birds[i].setDeltaPos(DEFAULTDP);
	}
}

void Flock::move() {
	for (uint i = 0; i < size; i++) {
		birds[i].updateDir(birds, size, eyeSight, randomJiggle, width, height);
	}
	for (uint i = 0; i < size; i++) {
		birds[i].move(width, height);
	}
}

int Flock::changeMode() {
	mode = (mode == MODE1) ? MODE2 : MODE1;
	if (mode == MODE1) {
		for (int i = 0; i < size; i++) {
			birds[i].setDeltaPos(DEFAULTDP);
		}
	}
	else {
		for (int i = 0; i < size; i++) {
			double dp = (double)rand() / (double)RAND_MAX * (MAXDP - MINDP) + MINDP;
			birds[i].setDeltaPos(dp);
		}
	}
	int ret = (mode == MODE1) ? 1 : 2;
	return ret;
}

double Flock::increaseEyeSight() {
	eyeSight += DELTAES;
	return eyeSight;
}

double Flock::decreaseEyeSight() {
	eyeSight -= DELTAES;
	if (eyeSight < DELTAES) eyeSight = DELTAES;
	return eyeSight;
}

double Flock::increaseJiggle() {
	randomJiggle += DELTARJ;
	return randomJiggle;
}

double Flock::decreaseJiggle() {
	randomJiggle -= DELTARJ;
	if (randomJiggle < DELTARJ) randomJiggle = DELTARJ;
	return randomJiggle;
}