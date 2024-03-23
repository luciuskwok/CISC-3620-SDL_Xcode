// vector.c

// Sources:
// Rotation calculations based on https://msl.cs.uiuc.edu/planning/node102.html
// Matrix multiplication based on https://mathinsight.org/matrix_vector_multiplication


#include "vector.h"
#include <math.h>

#pragma mark - 2D Vector

vec2_t vec2_add(vec2_t a, vec2_t b) {
	vec2_t c = { a.x + b.x, a.y + b.y };
	return c;
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
	vec2_t c = { a.x - b.x, a.y - b.y };
	return c;
}

vec2_t vec2_mul(vec2_t a, float b) {
	vec2_t c = { a.x * b, a.y * b };
	return c;
}

vec2_t vec2_div(vec2_t a, float b) {
	vec2_t c = { a.x / b, a.y / b };
	return c;
}

vec2_t vec2_rotate(vec2_t p, float a) {
	vec2_t q;
	q.x = p.x * cosf(a) - p.y * sinf(a);
	q.y = p.x * sinf(a) + p.y * cosf(a);
	return q;
}

float vec2_length(vec2_t v) {
    return hypotf(v.x, v.y);
}

#pragma mark - 3D Vector

vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t c = { a.x + b.x, a.y + b.y, a.z + b.z };
	return c;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t c = { a.x - b.x, a.y - b.y, a.z - b.z };
	return c;
}

vec3_t vec3_mul(vec3_t a, float b) {
	vec3_t c = { a.x * b, a.y * b, a.z * b };
	return c;
}

vec3_t vec3_div(vec3_t a, float b) {
	vec3_t c = { a.x / b, a.y / b, a.z / b };
	return c;
}

float vec3_length(vec3_t v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
