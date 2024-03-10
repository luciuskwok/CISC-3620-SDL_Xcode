//
//  mesh.h
//  SDL_Xcode
//
//  Created by Lucius Kwok on 3/10/24.
//

#ifndef MESH_H
#define MESH_H

#include "vector.h"

// Face (triangle)
typedef struct {
    int a, b, c;
} face_t;


// Number of points in the mesh
#define M_MESH_VERTICES (8)

// Declare array here but use extern because it will be initialized in mesh.c
extern vec3_t cube_vertices[M_MESH_VERTICES];

// Number of faces: 6 for each cube face * 2 for triangles per face
#define M_MESH_FACES (6 * 2)

// Declare array of faces
extern face_t mesh_faces[M_MESH_FACES];

#endif /* MESH_H */
