//
//  mesh.c
//  SDL_Xcode
//
//  Created by Lucius Kwok on 3/10/24.
//

#include "mesh.h"

/*
 Vertex numbering
   6--4
  /| /|
 1--2 |
 | 7|-5
 |/ |/
 0--3
 */

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



