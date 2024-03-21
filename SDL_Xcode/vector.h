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

// 2D Functions
vec2_t vec2_rotate(vec2_t p, float a);
float vec2_length(vec2_t v);

// 3D Functions
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_subtract(vec3_t a, vec3_t b);
float vec3_length(vec3_t v);

#endif /* VECTOR_H */
