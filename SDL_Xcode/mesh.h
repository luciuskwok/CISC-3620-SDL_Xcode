//
//  mesh.h
//  SDL_Xcode
//
//  Created by Lucius Kwok on 3/10/24.
//

#ifndef MESH_H
#define MESH_H

#include "color.h"
#include "matrix.h"
#include "vector.h"

#include <stdint.h>

typedef struct {
	vec3_t a, b, c;
} mesh_face_t;

// Properties
typedef struct {
	// Geometry
	int face_count;
	mesh_face_t *faces;
	
	// Visuals
	color_abgr_t line_color;
	color_abgr_t point_color;

	// Physics
	mat4_t rotation;
	vec3_t scale;
	vec3_t position; // meters
	vec3_t linear_momentum; // meters/second
	vec3_t angular_momentum; // degrees/second
	double lifetime;
} mesh_t;


// Functions
mesh_t *mesh_new_cube(void);

mesh_t *mesh_new(int faces);
void mesh_destroy(mesh_t *mesh);
void mesh_update(mesh_t *mesh, double delta_time);
void mesh_draw(mesh_t *mesh);

void mesh_add_pitch(mesh_t *mesh, float deg);
void mesh_add_roll(mesh_t *mesh, float deg);
void mesh_add_yaw(mesh_t *mesh, float deg);

#endif /* MESH_H */
