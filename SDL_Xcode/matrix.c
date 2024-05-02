//
//  matrix.c
//  SDL_Xcode
//
//  Created by Lucius Kwok on 3/21/24.
//

#include "matrix.h"
#include <math.h>

#pragma mark - 2D Matrix

mat3_t mat3_identity(void) {
	mat3_t m = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};
	return m;
}

mat3_t mat3_translate(mat3_t m, vec2_t t) {
	mat3_t n = {
		1, 0, t.x,
		0, 1, t.y,
		0, 0, 1
	};
	return mat3_mul(m, n);
}

mat3_t mat3_scale(mat3_t m, vec2_t s) {
	mat3_t n = {
		s.x, 0, 0,
		0, s.y, 0,
		0, 0, 1
	};
	return mat3_mul(m, n);
}

mat3_t mat3_rotate(mat3_t m, float a) {
	mat3_t n = {
		cosf(a), -sinf(a), 0,
		sinf(a), cosf(a), 0,
		0, 0, 1
	};
	return mat3_mul(m, n);
}

mat3_t mat3_mul(const mat3_t a, const mat3_t b) {
	mat3_t c = {0};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			c.m[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				c.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return c;
}

vec2_t vec2_mat3_mul(const vec2_t v, const mat3_t m) {
	vec2_t b;
	b.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2];
	b.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2];
	//float w = m[2][0] * a.x + m[2][1] * a.y + m[2][2];
	return b;
}

vec3_t vec3_mat3_mul(const vec3_t a, const mat3_t m) {
	vec3_t b;
	b.x = m.m[0][0] * a.x + m.m[0][1] * a.y + m.m[0][2] * a.z;
	b.y = m.m[1][0] * a.x + m.m[1][1] * a.y + m.m[1][2] * a.z;
	b.z = m.m[2][0] * a.x + m.m[2][1] * a.y + m.m[2][2] * a.z;
	return b;
}

#pragma mark - 3D Matrix


mat4_t mat4_identity(void) {
	mat4_t m = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return m;
}

mat4_t mat4_translate(mat4_t m, vec3_t t) {
	mat4_t n = {
		1, 0, 0, t.x,
		0, 1, 0, t.y,
		0, 0, 1, t.z,
		0, 0, 0, 1
	};
	return mat4_mul(m, n);
}

mat4_t mat4_scale(mat4_t m, vec3_t s) {
	mat4_t n = {
		s.x, 0, 0, 0,
		0, s.y, 0, 0,
		0, 0, s.z, 0,
		0, 0, 0, 1
	};
	return mat4_mul(m, n);
}

mat4_t mat4_rot_x(mat4_t m, float a) {
	mat4_t n = {
		1, 0, 0, 0,
		0, cosf(a), -sinf(a), 0,
		0, sinf(a), cosf(a), 0,
		0, 0, 0, 1
	};
	return mat4_mul(m, n);
}

mat4_t mat4_rot_y(mat4_t m, float a) {
	mat4_t n = {
		 cosf(a), 0, sinf(a), 0,
		 0, 1, 0, 0,
		 -sinf(a), 0, cosf(a), 0,
		 0, 0, 0, 1
	 };
	return mat4_mul(m, n);
}

mat4_t mat4_rot_z(mat4_t m, float a) {
	mat4_t n = {
		cosf(a), -sinf(a), 0, 0,
		sinf(a), cosf(a), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return mat4_mul(m, n);
}

mat4_t mat4_apply_euler_angles(mat4_t m, vec3_t a) {
	m = mat4_rot_x(m, a.x);
	m = mat4_rot_z(m, a.z);
	m = mat4_rot_y(m, a.y);
	return m;
}

mat4_t mat4_mul(const mat4_t a, const mat4_t b) {
	mat4_t c  = {0};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			c.m[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				c.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return c;
}

vec3_t vec3_mat4_mul(const vec3_t v, const mat4_t m) {
	vec4_t a = vec3_to_vec4(v);
	vec4_t b = vec4_mat4_mul(a, m);
	return vec4_to_vec3(b);
}

vec4_t vec4_mat4_mul(const vec4_t v, const mat4_t m) {
	float b[4] = { v.x, v.y, v.z, v.w };
	float c[4] = { 0, 0, 0, 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			c[i] += m.m[i][j] * b[j];
		}
	}
	vec4_t result = { c[0], c[1], c[2], c[3] };
	return result;
}
