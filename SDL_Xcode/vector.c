// vector.c

// Sources:
// Rotation calculations based on https://msl.cs.uiuc.edu/planning/node102.html
// Matrix multiplication based on https://mathinsight.org/matrix_vector_multiplication


#include "vector.h"
#include <math.h>


vec2_t vec2_rotate(vec2_t p, float a) {
	vec2_t q;
	q.x = p.x * cosf(a) - p.y * sinf(a);
	q.y = p.x * sinf(a) + p.y * cosf(a);
	return q;
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

vec3_t vec3_subtract(vec3_t a, vec3_t b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return a;
}

void mat3_get_identity(mat3_t m) {
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
}

void mat3_translate(mat3_t m, float tx, float ty) {
	mat3_t t = {
		1, 0, tx,
		0, 1, ty,
		0, 0, 1
	};
	mat3_multiply(m, t, m);
}

void mat3_scale(mat3_t m, float sx, float sy) {
	mat3_t s = {
		sx, 0, 0,
		0, sy, 0,
		0, 0, 1
	};
	mat3_multiply(m, s, m);
}

void mat3_rotate(mat3_t m, float a) {
	mat3_t r = {
		cosf(a), -sinf(a), 0,
		sinf(a), cos(a), 0,
		0, 0, 1
	};
	mat3_multiply(m, r, m);
}

void mat3_multiply(const mat3_t a, const mat3_t b, mat3_t result) {
	// Store results in a temporary matrix before copying to results
	mat3_t tmp;

	// Store calculations in tmp first
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			tmp[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				tmp[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	// Copy results from tmp to result
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result[i][j] = tmp[i][j];
		}
	}
}

vec2_t vec2_mat3_multiply(const vec2_t a, const mat3_t m) {
	vec2_t b;
	float w;
	b.x = m[0][0] * a.x + m[0][1] * a.y + m[0][2];
	b.y = m[1][0] * a.x + m[1][1] * a.y + m[1][2];
	w = m[2][0] * a.x + m[2][1] * a.y + m[2][2];
	return b;
}


void mat4_get_identity(mat4_t m) {
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 1; m[3][3] = 1;
}

void mat4_translate(mat4_t m, float tx, float ty, float tz) {
	mat4_t t = {
		1, 0, 0, tx,
		0, 1, 0, ty,
		0, 0, 1, tz,
		0, 0, 0, 1
	};
	mat4_multiply(m, t, m);
}

void mat4_scale(mat4_t m, float sx, float sy, float sz) {
	mat4_t s = {
		sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1
	};
	mat4_multiply(m, s, m);
}

void mat4_pitch(mat4_t m, float a) {
	mat4_t r = {
		cosf(a), 0, sinf(a), 0,
		0, 1, 0, 0,
		-sinf(a), 0, cosf(a), 0,
		0, 0, 0, 1
	};
	mat4_multiply(m, r, m);
}

void mat4_roll(mat4_t m, float a) {
	mat4_t r = {
		1, 0, 0, 0,
		0, cosf(a), -sinf(a), 0,
		0, sinf(a), cosf(a), 0,
		0, 0, 0, 1
	};
	mat4_multiply(m, r, m);
}

void mat4_yaw(mat4_t m, float a) {
	mat4_t r = {
		cosf(a), -sinf(a), 0, 0,
		sinf(a), cosf(a), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	mat4_multiply(m, r, m);
}

void mat4_multiply(const mat4_t a, const mat4_t b, mat4_t result) {
	// Store results in a temporary matrix before copying to results
	mat4_t tmp;

	// Store calculations in tmp first
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				tmp[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	// Copy results from tmp to result
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = tmp[i][j];
		}
	}
}

vec3_t vec3_mat4_multiply(const vec3_t a, const mat4_t m) {
	float b[4] = { a.x, a.y, a.z, 1 };
	float c[4] = { 0, 0, 0, 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			c[i] += m[i][j] * b[j];
		}
	}
	vec3_t result;
	result.x = c[0];
	result.y = c[1];
	result.z = c[2];
	return result;
}


