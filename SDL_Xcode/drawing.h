//
//  drawing.h
//  Toma Boxing
//
//  Created by Lucius Kwok on 3/24/24.
//

#ifndef drawing_h
#define drawing_h

#include "color.h"
#include "matrix.h"
#include "vector.h"

#include <stdbool.h>

// Drawing context
extern color_abgr_t line_color;
extern color_abgr_t fill_color;
extern vec2_t cursor;

// Transforms
extern mat3_t view_transform_2d;
extern mat4_t camera_transform_3d;

// SDL Interface
bool init_screen(int width, int height, int scale);
void destroy_screen(void);
void render_to_screen(void);

// Drawing 2D
void fill_screen(void);

void move_to(vec2_t a);
void line_to(vec2_t a);
void fill_rect(int x, int y, int w, int h);
void fill_centered_rect(int x, int y, int w, int h);

void set_pixel(int x, int y, color_abgr_t color);

int get_screen_width(void);
int get_screen_height(void);

// Projection 3D
void init_projection(void);
vec2_t orthographic_project_point(vec3_t pt3d);
vec2_t perspective_project_point(vec3_t pt3d);
vec3_t get_camera_position(void);

#endif /* drawing_h */
