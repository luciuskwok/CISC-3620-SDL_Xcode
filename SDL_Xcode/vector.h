// vector.h

#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

// Basic vector types

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

// Matrix types

typedef float mat3_t[3][3];
typedef float mat4_t[4][4];


// Functions

// 2D Functions
vec2_t vec2_rotate(vec2_t p, float a);
float vec2_length(vec2_t v);

// 3D Functions
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_subtract(vec3_t a, vec3_t b);
float vec3_length(vec3_t v);

// 2D Matrix Functions
void mat3_get_identity(mat3_t m);
void mat3_translate(mat3_t m, float tx, float ty);
void mat3_scale(mat3_t m, float sx, float sy);
void mat3_rotate(mat3_t m, float a);
void mat3_multiply(const mat3_t a, const mat3_t b, mat3_t result);
vec2_t vec2_mat3_multiply(const vec2_t a, const mat3_t m);

// 3D Matrix Functions
void mat4_get_identity(mat4_t m);
void mat4_translate(mat4_t m, float tx, float ty, float tz);
void mat4_scale(mat4_t m, float sx, float sy, float sz);
void mat4_pitch(mat4_t m, float a);
void mat4_roll(mat4_t m, float a);
void mat4_yaw(mat4_t m, float a);
void mat4_multiply(const mat4_t a, const mat4_t b, mat4_t result);
vec3_t vec3_mat4_multiply(const vec3_t a, const mat4_t m);

#endif /* VECTOR_H */
