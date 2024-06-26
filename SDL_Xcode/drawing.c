//
//  drawing.c
//  Toma Boxing
//
//  Created by Lucius Kwok on 3/24/24.
//

#include "drawing.h"
#include "color.h"
#include "mesh.h"
#include "vector.h"

#include <SDL2/SDL.h>
#include <stdio.h>


// Globals for SDL
SDL_Window* sdl_window;
SDL_Rect window_rect;
SDL_Renderer* sdl_renderer;
SDL_Texture* sdl_texture;
uint32_t* screen_pixels;
int screen_w;
int screen_h;
size_t screen_pitch;

// Drawing context
color_abgr_t line_color;
color_abgr_t fill_color;
vec2_t cursor;

// Transforms
mat3_t view_transform_2d;
mat4_t camera_transform_3d;
mat4_t perspective_matrix;


#pragma mark - SDL Interface

void init_projection(void) {
	// Maps the top of the screen to y = 1.0, bottom to y = -1.0,
	// and the sides vary based on the aspect ratio.
	// Origin is in the center.
	vec2_t st = { .x = screen_w / 2, .y = screen_h / 2 };
	view_transform_2d = mat3_translate(mat3_identity(), st);
	vec2_t s = { .x = screen_h / 2, .y = screen_h / 2 } ;
	view_transform_2d = mat3_scale(view_transform_2d, s);
	
	// Camera transform
	vec3_t ct = { .x = 0, .y = 0, .z = 5 };
	camera_transform_3d = mat4_translate(mat4_identity(), ct);
	
	// Perspective matrix
	float aspect = (float)screen_h / (float)screen_w;
	float fov = 60.0f * (float)M_PI / 180.0f;
	perspective_matrix = mat4_perspective_matrix(fov, aspect, 0.3f, 1000.0f);
}

bool init_screen(int width, int height, int scale) {
	//fprintf(stdout, "initialize_windowing_system().\n");
	
	// Set up SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Init(SDL_INIT_VIDEO) failed: %s\n", SDL_GetError());
		return false;
	}
	
	// Store dimensions in globals
	screen_w = width;
	screen_h = height;
	screen_pitch = (size_t)width * sizeof(uint32_t);
	window_rect.x = window_rect.y = 0;
	window_rect.w = width * scale;
	window_rect.h = height * scale;

	// Window & Renderer
	if (SDL_CreateWindowAndRenderer(window_rect.w, window_rect.h, 0, &sdl_window, &sdl_renderer) != 0) {
		fprintf(stderr, "SDL_CreateWindowAndRenderer() failed: %s\n", SDL_GetError());
		return false;
	}

	// Texture: Using ABGR pixel format is slightly faster (~10%) than using RGBA.
	sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!sdl_texture) {
		fprintf(stderr, "SDL_CreateTexture() failed: %s\n", SDL_GetError());
		return false;
	}

	// Allocate frame buffer
	screen_pixels = (uint32_t*)malloc((size_t)(height) * screen_pitch);
	if (!screen_pixels) {
		fprintf(stderr, "malloc() failed!\n");
		return false;
	}
	fill_screen(ABGR_BLACK);
	
	// Set up the renderer
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0); // Use no interpolation
	// Clear the window
	SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl_renderer);
	SDL_RenderPresent(sdl_renderer);

	// Debug logging: window * texture size
	fprintf(stdout, "Created window (%dx%d) and texture (%dx%d).\n", window_rect.w, window_rect.h, screen_w, screen_h);
	
	// Set up default transforms
	init_projection();

	return true;
}

void destroy_screen(void) {
	free(screen_pixels);
	SDL_DestroyTexture(sdl_texture);
	SDL_DestroyRenderer(sdl_renderer);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
}

void render_to_screen(void) {
	// Render frame buffer
	SDL_UpdateTexture(sdl_texture, NULL, screen_pixels, (int)screen_pitch);
	SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, &window_rect);
	SDL_RenderPresent(sdl_renderer);
}

#pragma mark - Drawing 2D

void fill_screen(color_abgr_t color) {
	for (int i = 0; i < screen_w * screen_h; i++) {
		screen_pixels[i] = color;
	}
}

void move_to(vec2_t a) {
	cursor = a;
}

void line_to(vec2_t a) {
	float dx = a.x - cursor.x;
	float dy = a.y - cursor.y;
	float steps = fabsf(dx) > fabsf(dy)? fabsf(dx) : fabsf(dy);
	float sx = dx / steps;
	float sy = dy / steps;
	float x = cursor.x;
	float y = cursor.y;
	for (float i = 0.0f; i <= steps; i++) {
		set_pixel((int)floorf(x), (int)floorf(y), line_color);
		x += sx;
		y += sy;
	}
	cursor = a;
}

void fill_rect(int x, int y, int w, int h) {
	for (int y1 = 0; y1 < h; y1++) {
		for (int x1 = 0; x1 < w; x1++) {
			set_pixel(x + x1, y + y1, fill_color);
		}
	}
}

void fill_centered_rect(int x, int y, int w, int h) {
	fill_rect(x - w / 2, y - h / 2, w, h);
}

void set_pixel(int x, int y, color_abgr_t color) {
	if (x < 0 || x >= screen_w) return;
	if (y < 0 || y >= screen_h) return;
	
	// Apply blending if color's alpha < 255
	int i = x + y * screen_w;
	if ((color & 0xFF000000) != 0xFF000000) {
		color = blend_color(screen_pixels[i], color);
	}
	screen_pixels[i] = color;
}

#pragma mark - Projection 3D

vec2_t orthographic_project_point(vec3_t pt3d) {
	// Drop z
	vec2_t pt2d = { .x = pt3d.x, .y = pt3d.y };

	// Apply view transform
	pt2d = vec2_mat3_mul(pt2d, view_transform_2d);
	return pt2d;
}

vec2_t perspective_project_point(vec3_t pt3d) {
	// Apply 3d transforms
	pt3d = vec3_mat4_mul(pt3d, camera_transform_3d);

	vec2_t pt2d = { .x = pt3d.x / pt3d.z, .y = pt3d.y / pt3d.z };

	// Apply view transform
	pt2d = vec2_mat3_mul(pt2d, view_transform_2d);

	return pt2d;
}

vec3_t get_camera_position(void) {
	vec3_t a = { 0, 0, 0 };
	vec3_t b = vec3_mat4_mul(a, camera_transform_3d);
	return vec3_sub(a, b);
}
