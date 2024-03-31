//
//  cube_mesh.c
//  Toma Boxing
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
uint64_t cube_ticks;

// Projected points
triangle_t projected_triangles[M_MESH_FACES];
vec2_t projected_points[M_MESH_VERTICES];


#pragma mark - Fuctions

void init_cube(void) {
	mat4_get_identity(cube_transform_3d);
	cube_pitch = cube_roll = cube_yaw = 0;
	cube_ticks = 0;
}

void update_cube(uint64_t delta_time) {
	// Update rotation
	float increment = ((float)M_PI / 180.f) * (float)delta_time / 1000.f; // 1 deg/sec
	mat4_pitch(cube_transform_3d, cube_pitch * increment);
	mat4_roll(cube_transform_3d, cube_roll * increment);
	mat4_yaw(cube_transform_3d, cube_yaw * increment);

	// Update triangle colors
	cube_ticks += delta_time;
	int hue = (cube_ticks / 16) % 360;
	cube_line_color = color_from_hsv(hue, 1.0, 1.0, 1.0);
	cube_point_color = color_from_hsv((hue + 60) % 360, 1.0, 1.0, 0.5);
}

void draw_cube(void) {
	vec3_t a3, b3, c3;
	vec2_t a2, b2, c2;
	vec3_t vab, vac, normal, camera_ray;
	float dot_normal_camera;
	const vec3_t camera_pos = get_camera_position();
	const int point_w = 5;
	
	set_line_color(cube_line_color);
	set_fill_color(cube_point_color);
	
	for (int i = 0; i < M_MESH_FACES; i++) {
		a3 = vec3_mat4_multiply(cube_vertices[cube_faces[i].a], cube_transform_3d);
		b3 = vec3_mat4_multiply(cube_vertices[cube_faces[i].b], cube_transform_3d);
		c3 = vec3_mat4_multiply(cube_vertices[cube_faces[i].c], cube_transform_3d);
		
		// Backface culling
		vab = vec3_sub(b3, a3);
		vac = vec3_sub(c3, a3);
		normal = vec3_cross(vab, vac);
		camera_ray = vec3_sub(a3, camera_pos);
		dot_normal_camera = vec3_dot(camera_ray, normal);
		
		if (dot_normal_camera > 0.0) {
			// Project to 2D
			a2 = perspective_project_point(a3);
			b2 = perspective_project_point(b3);
			c2 = perspective_project_point(c3);
			
			// Lines
			move_to(a2);
			line_to(b2);
			line_to(c2);
			line_to(a2);
			
			// Points
			fill_centered_rect((int)a2.x, (int)a2.y, point_w, point_w);
			fill_centered_rect((int)b2.x, (int)b2.y, point_w, point_w);
			fill_centered_rect((int)c2.x, (int)c2.y, point_w, point_w);
		}
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
