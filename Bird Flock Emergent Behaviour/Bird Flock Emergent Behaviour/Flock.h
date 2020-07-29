#pragma once

#include "Tipos.h"
#include "Bird.h"

#define MINDP		(double)0.2
#define	MAXDP		(double)3.0
#define	DEFAULTDP	1.0						//Delta Position
#define DELTAES		1						//Delta EyeSight
#define DELTARJ		1						//Delta RandomJiggle


enum {MODE1, MODE2};

class Flock {
public:
	Flock(uint n, double e, double j, uint w, uint h);
	~Flock() { delete[] birds; }
	void move();
	uint getSize() { return size; }
	Bird getBird(uint i) { return birds[i]; }
	uint getWidth() { return width; }
	uint getHeight() { return height; }
	double getEyeSight() { return eyeSight; }
	double getJiggle() { return randomJiggle; }
	void setEyeSight(double newSight) { eyeSight = newSight; }
	void setRandomJiggle(double newJiggle) { randomJiggle = newJiggle; }
	int changeMode();
	double increaseEyeSight();
	double decreaseEyeSight();
	double increaseJiggle();
	double decreaseJiggle();
private:
	uint size, width, height, mode;
	double eyeSight;
	double randomJiggle;
	Bird *birds;
};