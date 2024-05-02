//
//  matrix.h
//  CISC -3620-Midterm
//
//  Created by Lucius Kwok on 3/21/24.
//

#ifndef matrix_h
#define matrix_h

#include "vector.h"

// Matrix types

typedef float mat3_t[3][3];
typedef float mat4_t[4][4];

// 2D Matrix Functions
void mat3_identity(mat3_t m);
void mat3_translate(mat3_t m, float tx, float ty);
void mat3_scale(mat3_t m, float sx, float sy);
void mat3_rotate(mat3_t m, float a);
void mat3_multiply(const mat3_t a, const mat3_t b, mat3_t result);
vec3_t vec3_mat3_mul(const vec3_t a, const mat3_t m);
vec2_t vec2_mat3_mul(const vec2_t a, const mat3_t m);

// 3D Matrix Functions
void mat4_identity(mat4_t m);
void mat4_translate(mat4_t m, float tx, float ty, float tz);
void mat4_scale(mat4_t m, float sx, float sy, float sz);
void mat4_rot_x(mat4_t m, float a);
void mat4_rot_y(mat4_t m, float a);
void mat4_rot_z(mat4_t m, float a);
void mat4_mul(const mat4_t a, const mat4_t b, mat4_t result);
vec4_t vec4_mat4_mul(const vec4_t a, const mat4_t m);
vec3_t vec3_mat4_mul(const vec3_t a, const mat4_t m);

#endif /* matrix_h */
