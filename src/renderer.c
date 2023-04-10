#include "renderer.h"

renderer2d* renderer2d_create(iso_app* app) {
	renderer2d* ren = iso_alloc(sizeof(renderer2d));
	ren->app = app;

	ren->vb_cnt = 0;
	ren->ib_cnt = 0;

	// VBO
	ren->buffers.vbo = app->graphics->api.vertex_buffer_new(
		app->graphics,
		(iso_graphics_vertex_buffer_def) {
			.name = VBO,
			.size = MAX_VBO_SIZE,
			.data = NULL,
			.usage = ISO_GRAPHICS_DYNAMIC
		}
	);

	// IBO
	ren->buffers.ibo = app->graphics->api.index_buffer_new(
		app->graphics,
		(iso_graphics_index_buffer_def) {
			.name = IBO,
			.size = MAX_IBO_SIZE,
			.data = NULL,
			.usage = ISO_GRAPHICS_DYNAMIC
		}
	);

	// Shader
	ren->buffers.shader = app->graphics->api.shader_new(
		app->graphics,
		(iso_graphics_shader_def) {
			.name = SHADER,
			.f_src = FRAG_SHADER,
			.v_src = VERT_SHADER,
			.source_type = ISO_SHADER_FROM_FILE
		}
	);

	// Creating pipline
	ren->buffers.pip = app->graphics->api.render_pipeline_new(
		app->graphics,
		(iso_graphics_render_pipeline_def) {
			.name = PIP,
			.buffers = {
				.vbo = ren->buffers.vbo,
				.ibo = ren->buffers.ibo,
				.shader = ren->buffers.shader
			},

			.amt = 2,
			.layout = (iso_graphics_vertex_layout_def[]) {
				{ .amt = 3, .type = ISO_GRAPHICS_FLOAT },
				{ .amt = 4, .type = ISO_GRAPHICS_FLOAT },
			}
		}
	);

	return ren;
}

void renderer2d_delete(renderer2d* ren) {
	ren->app->graphics->api.vertex_buffer_delete(ren->app->graphics, VBO);
	ren->app->graphics->api.index_buffer_delete(ren->app->graphics, IBO);
	ren->app->graphics->api.shader_delete(ren->app->graphics, SHADER);
	ren->app->graphics->api.render_pipeline_delete(ren->app->graphics, PIP);
	iso_free(ren);
}

void renderer2d_begin(renderer2d* ren) {
	ren->vb_cnt = 0;
	ren->ib_cnt = 0;
	ren->app->graphics->api.render_pipeline_begin(ren->app->graphics, PIP);
}

void renderer2d_end(renderer2d* ren) {
	ren->app->graphics->api.vertex_buffer_update(
		ren->app->graphics,
		VBO,
		(iso_graphics_buffer_update_def) {
			.start_sz = 0,
			.end_sz   = ren->vb_cnt * sizeof(f32),
			.data     = ren->vb
		}
	);

	ren->app->graphics->api.index_buffer_update(
		ren->app->graphics,
		IBO,
		(iso_graphics_buffer_update_def) {
			.start_sz = 0,
			.end_sz   = ren->ib_cnt * sizeof(f32),
			.data     = ren->ib
		}
	);

	ren->app->graphics->api.render_pipeline_end(
		ren->app->graphics,
		PIP,
		ren->ib_cnt
	);
}

void renderer2d_push_mesh(renderer2d* ren, mesh2d* mesh) {
	if (mesh->vertex_cnt >= (MAX_BUFF_SIZE - ren->vb_cnt)) {
		renderer2d_end(ren);
		renderer2d_begin(ren);
	}

	// Filling index buffer
	for (i32 i = 0; i < mesh->index_cnt; i++) {
		ren->ib[ren->ib_cnt++] = ren->vb_cnt + mesh->indices[i];
	}

	// Filling the vertex buffer
	for (i32 i = 0; i < mesh->vertex_cnt; i++) {
		iso_vec3 p = mesh->vertices[i];
		ren->vb[ren->vb_cnt++] = p.x;
		ren->vb[ren->vb_cnt++] = p.y;
		ren->vb[ren->vb_cnt++] = p.z;
		ren->vb[ren->vb_cnt++] = mesh->color.r;
		ren->vb[ren->vb_cnt++] = mesh->color.g;
		ren->vb[ren->vb_cnt++] = mesh->color.b;
		ren->vb[ren->vb_cnt++] = mesh->color.a;
	}
}

