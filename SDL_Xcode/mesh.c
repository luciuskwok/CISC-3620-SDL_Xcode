//
//  mesh.c
//  SDL_Xcode
//
//  Created by Lucius Kwok on 3/10/24.
//

#include "mesh.h"
#include "drawing.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

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
#define CUBE_VERTICES_LEN (8)
vec3_t cube_vertices[CUBE_VERTICES_LEN] = {
	{ -1, -1, -1 },
	{ -1,  1, -1 },
	{  1,  1, -1 },
	{  1, -1, -1 },
	{  1,  1,  1 },
	{  1, -1,  1 },
	{ -1,  1,  1 },
	{ -1, -1,  1 }
};

// Number of faces: 6 for each cube face * 2 for triangles per face
#define CUBE_FACES_LEN (12)
face_t cube_faces[CUBE_FACES_LEN] = {
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

// Projected points
#define PROJECTED_POINTS_LEN (256)
vec2_t projected_points[PROJECTED_POINTS_LEN];


#pragma mark -

mesh_t *mesh_new_cube(void) {
	mesh_t *mesh = mesh_new(CUBE_FACES_LEN);
	if (!mesh) return NULL;

	mesh_face_t *f = mesh->faces;
	for (int i=0; i<CUBE_FACES_LEN; i++) {
		f[i].a = cube_vertices[cube_faces[i].a];
		f[i].b = cube_vertices[cube_faces[i].b];
		f[i].c = cube_vertices[cube_faces[i].c];
	}
	
	return mesh;
}

mesh_t *mesh_new(int faces) {
	mesh_t *mesh = malloc(sizeof(mesh_t));
	if (!mesh) return NULL;

	// Geometry
	mesh->face_count = faces;
	mesh->faces = NULL;
	if (faces > 0) {
		mesh->faces = malloc(sizeof(mesh_face_t) * (size_t)faces);
	}
	
	// Visuals
	mesh->line_color = ABGR_WHITE;
	mesh->point_color = 0;

	// Physics
	mesh->rotation = mat4_identity();
	mesh->scale = vec3_make(1, 1, 1);
	mesh->position = vec3_zero();
	mesh->linear_momentum = vec3_zero();
	mesh->angular_momentum = vec3_zero();
	mesh->lifetime = 0.0;

	return mesh;
}

void cube_destroy(mesh_t *mesh) {
	if (mesh->faces) free(mesh->faces);
	free(mesh);
}

void mesh_update(mesh_t *mesh, double delta_time) {
	mesh->lifetime += delta_time;
	
	// Update rotation
	vec3_t rad = vec3_mul(mesh->angular_momentum, (float)(M_PI / 180.0 * delta_time));
	mat4_t increment = mat4_apply_euler_angles(mat4_identity(), rad);
	mesh->rotation = mat4_mul(mesh->rotation, increment);
	
	// Update position
	mesh->position = vec3_add(mesh->position, mesh->linear_momentum);
}


void mesh_draw(mesh_t *mesh) {
	// Tranformation matrix
	mat4_t transform = mat4_identity();
	transform = mat4_translate(transform, mesh->position);
	transform = mat4_mul(transform, mesh->rotation);
	transform = mat4_scale(transform, mesh->scale);

	if (mesh->face_count > 0 && mesh->faces) {
		vec3_t a3, b3, c3;
		vec2_t a2, b2, c2;
		vec3_t vab, vac, normal, camera_ray;
		float dot_normal_camera;
		const vec3_t camera_pos = get_camera_position();
		const int point_w = 3;
		
		// Color
		line_color = mesh->line_color;
		fill_color = mesh->point_color;
		
		for (int i = 0; i < mesh->face_count; i++) {
			mesh_face_t face = mesh->faces[i];
			
			a3 = vec3_mat4_mul(face.a, transform);
			b3 = vec3_mat4_mul(face.b, transform);
			c3 = vec3_mat4_mul(face.c, transform);
			
			// Backface culling
			vab = vec3_sub(b3, a3);
			vac = vec3_sub(c3, a3);
			normal = vec3_cross(vab, vac);
			camera_ray = vec3_sub(a3, camera_pos);
			dot_normal_camera = vec3_dot(camera_ray, normal);
			bool should_draw = dot_normal_camera > 0.0;
			
			if (should_draw) {
				// Project to 2D
				a2 = perspective_project_point(a3);
				b2 = perspective_project_point(b3);
				c2 = perspective_project_point(c3);
				
				// Lines
				if (mesh->line_color != 0) {
					move_to(a2);
					line_to(b2);
					line_to(c2);
					line_to(a2);
				}
				
				// Points
				if (mesh->point_color != 0) {
					fill_centered_rect((int)a2.x, (int)a2.y, point_w, point_w);
					fill_centered_rect((int)b2.x, (int)b2.y, point_w, point_w);
					fill_centered_rect((int)c2.x, (int)c2.y, point_w, point_w);
				}
			}
		}
	}
}

#pragma mark -

void mesh_add_pitch(mesh_t *mesh, float deg) {
	mesh->angular_momentum.x += deg;
}

void mesh_add_roll(mesh_t *mesh, float deg) {
	mesh->angular_momentum.z += deg;
}

void mesh_add_yaw(mesh_t *mesh, float deg) {
	mesh->angular_momentum.y += deg;
}
