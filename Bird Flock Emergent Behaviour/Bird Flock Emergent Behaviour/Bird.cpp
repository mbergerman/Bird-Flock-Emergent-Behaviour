
#define _USE_MATH_DEFINES
#include <math.h>

#include <stdlib.h>
#include "Tipos.h"
#include "Bird.h"

void Bird::setRandomPos(uint width, uint height) {
	x = ((double)rand() / (double)RAND_MAX) * width;
	y = ((double)rand() / (double)RAND_MAX) * height;
	dir = ((double)rand() / (double)RAND_MAX) * 2 * M_PI - M_PI;	//De -PI a +PI

	newdir = dir;
}

void Bird::move(uint width, uint height) {
	dir = newdir;

	x += deltapos * cos(dir);
	y += deltapos * sin(dir);

	if (x < 0)		x += width;
	if (x > width)	x -= width;
	if (y < 0)		y += height;
	if (y > height)	y -= height;
}

void Bird::updateDir(Bird* birds, uint n, double eyeSight, double randomJiggle, uint width, uint height){
	double xsum = 0, ysum = 0;
	for (uint i = 0; i < n; i++) {
		if (isInSight(birds[i], eyeSight, width, height)) {
			xsum += cos(birds[i].getDir() );
			ysum += sin(birds[i].getDir() );
		}
	}
	newdir = atan2(ysum, xsum);
	double radians_jiggle = (2 * M_PI / 360) * randomJiggle;
	newdir += ((double)rand() / (double)RAND_MAX) * 2 * radians_jiggle - radians_jiggle;
}

bool Bird::isInSight(Bird& bird, double eyeSight, uint width, uint height) {
	double deltax = fabs(x - bird.getX());
	double deltay = fabs(y - bird.getY());
	if (deltax > width / 2) deltax = width - deltax;
	if (deltay > height / 2) deltay = height - deltay;

	if (deltax * deltax + deltay * deltay <= eyeSight*eyeSight) {
		return true;
	}
	return false;
}