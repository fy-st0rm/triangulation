#include "isolate.h"
#include "triangulation.h"

static mesh2d* mesh;

iso_app_def iso_init() {
	return (iso_app_def) {
		.fps = 60.0f,
		.window_def = (iso_window_def) {
			.title  = "Renderer",
			.width  = 800,
			.height = 600
		},
		.graphics_def = (iso_graphics_def) {
			.api = ISO_OPENGL_API
		}
	};
}

void iso_start(iso_app* app) {
	iso_vec3 vertices[] = {
		//{ -0.3f, 0.3f, 0.0f },
		//{ -0.3f,-0.3f, 0.0f },
		//{  0.3f,-0.3f, 0.0f },
		//{  0.3f, 0.3f, 0.0f },
		{  0.0f,  0.5f, 0.0f },
		{ -0.4f,  0.3f, 0.0f },
		{  0.4f,  0.0f, 0.0f },
		{ -0.2f, -0.2f, 0.0f },
		{  0.2f, -0.5f, 0.0f },
		{  0.8f, -0.3f, 0.0f },
	};
	iso_color color = { 1, 0, 1, 1 };

	mesh = mesh2d_create(vertices, sizeof(vertices) / sizeof(vertices[0]), color);
	mesh2d_generate_indices(mesh);
}

void iso_update(iso_app* app, f32 dt) {
	app->graphics->api.clear_window(app->window, (iso_color) { 0, 0, 0, 1 });
}

void iso_event(iso_app* app, SDL_Event event) {
	if (event.type == SDL_QUIT) {
		app->state = ISO_APP_CLOSED;
	}
}

void iso_exit(iso_app* app) {
	mesh2d_delete(mesh);
}

int main(int argc, char** argv) {
	return iso_run(argc, argv);
}
