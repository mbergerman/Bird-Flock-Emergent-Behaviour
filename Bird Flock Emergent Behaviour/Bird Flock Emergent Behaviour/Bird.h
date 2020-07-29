#pragma once

#include "Tipos.h"

class Bird {
public:
	Bird() {};
	void move(uint width, uint height);
	void updateDir(Bird* birds, uint n, double eyeSight, double randomJiggle, uint width, uint height);
	double getX() { return x; }
	double getY() { return y; }
	double getDir() { return dir; }
	void setRandomPos(uint width, uint height);
	void setDeltaPos(double dp) { deltapos = dp; }
private:
	double x = 0, y = 0, dir = 0, newdir = 0, deltapos = 1;

	bool isInSight(Bird& bird, double eyeSight, uint width, uint height);
};