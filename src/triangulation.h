#ifndef __TRI_H__
#define __TRI_H__

#include "isolate.h"

//TODO: Generate dynamic array to store triangles


#define max(x, y) ((x) > (y)) ? (x) : (y)
#define min(x, y) ((x) < (y)) ? (x) : (y)

typedef struct {
	iso_vec3* vertices;
	u32 vertex_cnt;

	u32* indices;
	u32 index_cnt;
	u32 max_index_cnt;

	iso_color color;
} mesh2d;

typedef struct {
	iso_vec3 center;
	f32 radius;
} circle;

typedef struct {
	iso_vec3 a, b, c;
} triangle;

mesh2d* mesh2d_create(iso_vec3* vertices, u32 vertex_cnt, iso_color color);
void    mesh2d_delete(mesh2d* mesh);

void mesh2d_print(mesh2d* mesh);
void mesh2d_sort_vertices(mesh2d* mesh);
i32  mesh2d_search(mesh2d* mesh, iso_vec3 vec);
void mesh2d_add_vertex(mesh2d* mesh, iso_vec3 vertex);
void mesh2d_calculate_indices(mesh2d* mesh);

#endif // __TRI_H__
