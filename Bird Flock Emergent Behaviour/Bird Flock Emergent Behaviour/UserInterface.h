#pragma once

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "allegro5/allegro_image.h"
#include "Flock.h"

enum {
	ERR_NONE,
	ERR_AL_INIT,
	ERR_AL_PRIMITIVES,
	ERR_AL_IMAGE,
	ERR_AL_KEYBOARD,
	ERR_AL_TIMER,
	ERR_AL_DISPLAY,
	ERR_AL_EVQUEUE,
	ERR_AL_LOAD_BITMAP
};

class UserInterface {
public:
	UserInterface(int w, int h, int s, int& error);
	~UserInterface();
	void handleUI(Flock& flock, int bird_size);
	void printInstructions();
	void drawEyeSight(int x, int y, int e);
private:
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* event_queue;
	ALLEGRO_TIMER* timer;
	ALLEGRO_BITMAP* bird_img;
	double FPS;
	bool toggle_radius = false;
	int display_width, display_height;
	double display_scale;
};