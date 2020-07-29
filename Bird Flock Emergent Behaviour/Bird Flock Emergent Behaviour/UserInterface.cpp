#include <iostream>
#include "UserInterface.h"

UserInterface::UserInterface(int w, int h, int s, int& error) {
	display = NULL;
	event_queue = NULL;
	timer = NULL;
	bird_img = NULL;
	FPS = 30.0;
	display_width = w * s;
	display_height = h * s;
	display_scale = s;
	error = ERR_NONE;

	if (!al_init()) {
		error = ERR_AL_INIT;
	}
	else if (!al_init_primitives_addon()) {
		error = ERR_AL_PRIMITIVES;
	}
	else if (!al_init_image_addon()) {
		error = ERR_AL_IMAGE;
	}
	else if (!al_install_keyboard()) {
		error = ERR_AL_KEYBOARD;
	}
	else {
		timer = al_create_timer(1.0 / FPS);
		if (!timer) {
			error = ERR_AL_TIMER;
		}
		else {
			display = al_create_display(display_width, display_height);
			if (!display) {
				al_destroy_timer(timer);
				error = ERR_AL_DISPLAY;
			}
			else {
				event_queue = al_create_event_queue();
				if (!event_queue) {
					al_destroy_display(display);
					al_destroy_timer(timer);
					error = ERR_AL_EVQUEUE;
				}
				else {
					bird_img = al_load_bitmap("bird.png");
					if (!bird_img) {
						al_destroy_display(display);
						al_destroy_timer(timer);
						al_destroy_event_queue(event_queue);
						error = ERR_AL_LOAD_BITMAP;
					}
				}
			}
		}
	}
}

UserInterface::~UserInterface() {
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_bitmap(bird_img);
}

void UserInterface::handleUI(Flock& flock, int bird_size) {
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);

	bool ok = true;
	bool toggle_radius = false;
	bool redraw = true;

	while (ok)
	{
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;						//Ready to draw next frame
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_E:
				printf("EyeSight incrementado. EyeSight = %f\n", flock.increaseEyeSight());
				break;
			case ALLEGRO_KEY_D:
				printf("EyeSight decrementado. EyeSight = %f\n", flock.decreaseEyeSight());
				break;
			case ALLEGRO_KEY_R:
				printf("RandomJiggle incrementado. RandomJiggle = %f\n", flock.increaseJiggle());
				break;
			case ALLEGRO_KEY_F:
				printf("RandomJiggle decrementado. RandomJiggle = %f\n", flock.decreaseJiggle());
				break;
			case ALLEGRO_KEY_M:
				printf("Modo cambiado. Modo = %d\n", flock.changeMode());
				break;
			case ALLEGRO_KEY_N:
				toggle_radius = !toggle_radius;
				printf("Visualizacion del radio de vision ");
				printf("%s\n", (toggle_radius) ? "activada" : "desactivada");
				break;

			case ALLEGRO_KEY_T:
				FPS++;
				al_set_timer_speed(timer, 1.0 / FPS);
				printf("Velocidad incrementada. FPS = %d\n", (int)FPS);
				break;
			case ALLEGRO_KEY_G:
				if (FPS > 1) FPS--;
				al_set_timer_speed(timer, 1.0 / FPS);
				printf("Velocidad decrementada. FPS = %d\n", (int)FPS);
				break;

			case ALLEGRO_KEY_H:
				printInstructions();
				break;

			case ALLEGRO_KEY_ESCAPE:
				ok = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			ok = false;
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(100, 200, 255));

			if (toggle_radius) {
				double x = flock.getBird(0).getX() * display_scale;
				double y = flock.getBird(0).getY() * display_scale;
				double e = flock.getEyeSight() * display_scale;
				drawEyeSight(x, y, e);

				if (x - e < 0) {
					drawEyeSight(x + display_width, y, e);
					if (y - e < 0) drawEyeSight(x + display_width, y + display_height, e);
					if (y + e > display_height) drawEyeSight(x + display_width, y - display_height, e);
				}
				if (x + e > display_width) {
					drawEyeSight(x - display_width, y, e);
					if (y - e < 0) drawEyeSight(x - display_width, y + display_height, e);
					if (y + e > display_height) drawEyeSight(x - display_width, y - display_height, e);
				}
				if (y - e < 0) drawEyeSight(x, y + display_height, e);
				if (y + e > display_height) drawEyeSight(x, y - display_height, e);
			}

			for (uint i = 0; i < flock.getSize(); i++) {
				double x = flock.getBird(i).getX() * display_scale;
				double y = flock.getBird(i).getY() * display_scale;

				//al_draw_filled_circle(x, y, 5, al_map_rgb(0, 175, 0));
				al_draw_scaled_bitmap(bird_img, 0, 0, 100, 100, x - bird_size / 2, y - bird_size / 2, bird_size, bird_size, 0);
			}

			al_flip_display();

			flock.move();
		}
	}
}

void UserInterface::printInstructions() {
	printf("\nInstrucciones de uso:\n\n");
	printf("Presionar E para incrementar el EyeSight, D para decrementarlo\n");
	printf("Presionar R para incrementar el RandomJiggle, F para decrementarlo\n");
	printf("Presionar T para incrementar la velocidad, G para decrementarla\n");
	printf("Presionar M para cambiar de modo\n");
	printf("Presionar N para activar o desactivar la visualización del radio de visión\n");
	printf("Presionar H para volver a recibir las insctrucciones\n\n");
	printf("Presionar ESCAPE para terminar el programa\n\n");
}

void UserInterface::drawEyeSight(int x, int y, int e) {
	al_draw_circle(x, y, e, al_map_rgb(0, 0, 0), 3);
	al_draw_filled_circle(x, y, e, al_map_rgb(200, 200, 200));
}