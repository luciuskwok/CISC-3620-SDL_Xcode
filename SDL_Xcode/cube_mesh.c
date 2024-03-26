//
//  cube_mesh.c
//  SDL_Xcode
//
//  Created by Lucius Kwok on 3/10/24.
//

#include "cube_mesh.h"
#include "color.h"
#include "drawing.h"
#include "matrix.h"
#include <math.h>

/*
 Vertex numbering
   6--4
  /| /|
 1--2 |
 | 7|-5
 |/ |/
 0--3
 */


// Face (triangle)
typedef struct {
	int a, b, c;
} face_t;

typedef struct {
	vec2_t a, b, c;
} triangle_t;

// Number of points in the mesh
#define M_MESH_VERTICES (8)

// Number of faces: 6 for each cube face * 2 for triangles per face
#define M_MESH_FACES (6 * 2)

vec3_t cube_vertices[M_MESH_VERTICES] = {
	{ -1, -1, -1 },
	{ -1,  1, -1 },
	{  1,  1, -1 },
	{  1, -1, -1 },
	{  1,  1,  1 },
	{  1, -1,  1 },
	{ -1,  1,  1 },
	{ -1, -1,  1 }
};

face_t cube_faces[12] = {
	// front
	{ 0, 2, 1 },
	{ 0, 3, 2 },
	// right
	{ 2, 3, 4 },
	{ 3, 5, 4 },
	// back
	{ 4, 5, 6 },
	{ 5, 7, 6 },
	// left
	{ 0, 1, 6 },
	{ 0, 6, 7 },
	// top
	{ 1, 2, 6 },
	{ 2, 4, 6 },
	// bottom
	{ 0, 7, 3 },
	{ 3, 7, 5 }
};

// Cube parameters
mat4_t cube_transform_3d;
float cube_pitch, cube_roll, cube_yaw; // how many degrees per second of rotation
uint32_t cube_line_color;
uint32_t cube_point_color;

// Projected points
triangle_t projected_triangles[M_MESH_FACES];
vec2_t projected_points[M_MESH_VERTICES];


#pragma mark - Fuctions

void init_cube(void) {
	mat4_get_identity(cube_transform_3d);
	cube_pitch = cube_roll = cube_yaw = 0;
}

void update_cube(uint64_t frame_index) {
	// Update rotation
	float increment = (M_PI / 180.f) / 60.0f; // 1 deg/sec divided by 60 fps
	mat4_pitch(cube_transform_3d, cube_pitch * increment);
	mat4_roll(cube_transform_3d, cube_roll * increment);
	mat4_yaw(cube_transform_3d, cube_yaw * increment);

	// Update triangle colors
	int hue = frame_index % 360;
	cube_line_color = color_from_hsv(hue, 1.0, 1.0, 1.0);
	cube_point_color = color_from_hsv((hue + 60) % 360, 1.0, 1.0, 0.5);
}

vec2_t project_point(vec3_t a, mat4_t transform) {
	a = vec3_mat4_multiply(a, transform);
	return perspective_project_point(a);
}

void project_model(void) {
	// Project the 3d model into 2d space
	
	// Triangles
	for (int f = 0; f < M_MESH_FACES; f++) {
		projected_triangles[f].a = project_point(cube_vertices[cube_faces[f].a], cube_transform_3d);
		projected_triangles[f].b = project_point(cube_vertices[cube_faces[f].b], cube_transform_3d);
		projected_triangles[f].c = project_point(cube_vertices[cube_faces[f].c], cube_transform_3d);
	}
	
	// Points
	for (int i = 0; i < M_MESH_VERTICES; i++) {
		projected_points[i] = project_point(cube_vertices[i], cube_transform_3d);
	}
}

void draw_cube(void) {
	// Draw each triangle as lines for a wireframe
	triangle_t *tri;
	project_model();
	set_line_color(cube_line_color);
	for (int i = 0; i < M_MESH_FACES; i++) {
		tri = &projected_triangles[i];
		move_to(tri->a);
		line_to(tri->b);
		line_to(tri->c);
		line_to(tri->a);
	}
	
	// Draw each point as a 5x5 rectangle
	set_fill_color(cube_point_color);
	for (int i = 0; i < M_MESH_VERTICES; i++) {
		vec2_t pt = projected_points[i];
		fill_centered_rect(pt.x, pt.y, 5, 5);
	}
}

#pragma mark - Momemtum

void cube_reset_transform(void) {
	mat4_get_identity(cube_transform_3d);
	cube_pitch = cube_roll = cube_yaw = 0;
}

void cube_reset_momentum(void) {
	cube_pitch = cube_roll = cube_yaw = 0;
}

void cube_add_pitch(float x) {
	cube_pitch += x;
}

void cube_add_roll(float x) {
	cube_roll += x;
}

void cube_add_yaw(float x) {
	cube_yaw += x;
}

