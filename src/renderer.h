#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "triangulation.h"

#define VERT_SHADER "vert.glsl"
#define FRAG_SHADER "frag.glsl"

#define VBO "vbo"
#define IBO "ibo"
#define SHADER "shader"
#define PIP "pip"

#define VERTEX_SIZE 7
typedef struct {
	iso_vec3 pos;
	iso_color color;
} vertex;

#define MAX_QUAD_CNT  1000
#define MAX_VERT_CNT  MAX_QUAD_CNT  * 4
#define MAX_BUFF_SIZE MAX_VERT_CNT  * VERTEX_SIZE
#define MAX_VBO_SIZE  MAX_BUFF_SIZE * sizeof(f32)
#define MAX_IDX_CNT   MAX_QUAD_CNT  * 6
#define MAX_IBO_SIZE  MAX_IDX_CNT   * sizeof(u32)

typedef struct {
	iso_app* app;
	f32 vb[MAX_BUFF_SIZE];
	u32 vb_cnt;

	u32 ib[MAX_IDX_CNT];
	u32 ib_cnt;

	struct {
		iso_graphics_vertex_buffer* vbo;
		iso_graphics_index_buffer* ibo;
		iso_graphics_shader* shader;
		iso_graphics_render_pipeline* pip;
	} buffers;

} renderer2d;

renderer2d* renderer2d_create(iso_app* app);
void        renderer2d_delete(renderer2d* ren);

void renderer2d_begin(renderer2d* ren);
void renderer2d_end(renderer2d* ren);
void renderer2d_push_mesh(renderer2d* ren, mesh2d* mesh);

#endif // __RENDERER_H__
