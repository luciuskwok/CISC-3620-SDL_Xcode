// main.c

#include "color.h"
#include "drawing.h"
#include "mesh.h"
#include "vector.h"
#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


// Constants
#define FPS (30)
#define FRAME_TARGET_TIME (1000 / FPS)

// Globals
bool is_running = true;
uint64_t last_update_time = 0;
mesh_t *cube = NULL;

#pragma mark - Game Loop

void process_keyboard_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	// Keyboard interaction
	switch (event.type) {
	case SDL_QUIT: // when 'X' button is pressed in window titlebar
		// Exit program
		is_running = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				// Exit program
				fprintf(stdout, "Escape key pressed.\n");
				is_running = false;
				break;
			case SDLK_0:
				// Reset both angular momentum and rotation
				cube->angular_momentum = vec3_zero();
				cube->rotation = mat4_identity();
				break;
			case SDLK_e:
				// Roll right
				mesh_add_roll(cube, 1.0f);
				break;
			case SDLK_q:
				// Roll left
				mesh_add_roll(cube, -1.0f);
				break;
			case SDLK_w:
				// Pitch down
				mesh_add_pitch(cube, 1.0f);
				break;
			case SDLK_s:
				// Pitch up
				mesh_add_pitch(cube, -1.0f);
				break;
			case SDLK_a:
				// Yaw left
				mesh_add_yaw(cube, 1.0f);
				break;
			case SDLK_d:
				// Yaw right
				mesh_add_yaw(cube, -1.0f);
				break;
			case SDLK_SPACE:
				// Stop movement
				cube->angular_momentum = vec3_zero();
				break;
		}
		break;
	}
}

void update_state(uint64_t delta_time) {
	double delta_seconds = (double)delta_time / 1000.0;

	mesh_update(cube, delta_seconds);
	
	// Update cube colors
	double hue = fmod(cube->lifetime * 7.5, 360);
	cube->line_color = color_from_hsv(hue, 1.0, 1.0, 1.0);
	cube->point_color = color_from_hsv(fmod(hue + 60, 360), 1.0, 1.0, 0.5);
}

void run_render_pipeline(void) {
	fill_color = ABGR_BLACK;
	fill_screen();
	
	// Draw cube
	mesh_draw(cube);
	
	render_to_screen();
}

#pragma mark - Init & Clean Up

void run_game_loop(void) {
	// Run one iteration of game loop
	uint64_t update_start_time = SDL_GetTicks64();
	uint64_t delta_time = update_start_time - last_update_time;
	
	process_keyboard_input();
	if (!is_running) return;
	if (delta_time > 0) {
		update_state(delta_time);
	}
	run_render_pipeline();
	
#ifndef __EMSCRIPTEN__
	// Xcode version: delay to cap FPS
	uint64_t update_end_time = SDL_GetTicks64();
	uint64_t elapsed_time = update_end_time - last_update_time;
	if (elapsed_time < FRAME_TARGET_TIME) {
		SDL_Delay((uint32_t)(FRAME_TARGET_TIME - elapsed_time));
	} else if (elapsed_time > 200){
		fprintf(stdout, "Elapsed time: %llums.\n", elapsed_time);
	}

#endif
	last_update_time = update_start_time;
}

int main(int argc, const char * argv[]) {
	if (!init_screen(1280, 720, 1)) return 0;
	
	init_projection();
	cube = mesh_new_cube();
	
	last_update_time = SDL_GetTicks64();
	
#ifdef __EMSCRIPTEN__
	// WebAssembly version
	emscripten_set_main_loop(run_game_loop, 0, 1);
#else
	// Xcode version
	while (is_running) {
		run_game_loop();
	}
	destroy_screen();
#endif

	return 0;
}
