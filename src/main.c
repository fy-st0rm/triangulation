#include "isolate.h"
#include "triangulation.h"
#include "renderer.h"

static mesh2d* mesh;
static renderer2d* renderer;

iso_vec2 get_mouse_pos(iso_app* app) {
	i32 x, y;
	SDL_GetMouseState(&x, &y);
	f32 mx = (float)x / ((float)app->window->width / 2) - 1.0f;
	f32 my = 1 - (float)y / ((float)app->window->height/ 2);
	return (iso_vec2) { mx, my };
}

iso_app_def iso_init() {
	return (iso_app_def) {
		.fps = 60.0f,
		.window_def = (iso_window_def) {
			.title  = "Renderer",
			.width  = 800,
			.height = 600
		},
		.graphics_def = (iso_graphics_def) {
			.api = ISO_OPENGL_API,
		}
	};
}

void iso_start(iso_app* app) {
	iso_vec3 vertices[] = {
		{ -0.3f, 0.3f, 0.0f },
		{ -0.3f,-0.3f, 0.0f },
		{  0.3f,-0.3f, 0.0f },
		{  0.3f, 0.3f, 0.0f },
		//{  0.0f,  0.5f, 0.0f },
		//{ -0.4f,  0.3f, 0.0f },
		//{  0.4f,  0.0f, 0.0f },
		//{ -0.2f, -0.2f, 0.0f },
		//{  0.2f, -0.5f, 0.0f },
		//{  0.8f, -0.3f, 0.0f },
	};
	iso_color color = { 1, 0, 1, 1 };

	mesh = mesh2d_create(vertices, sizeof(vertices) / sizeof(vertices[0]), color);

	renderer = renderer2d_create(app);
}

void iso_update(iso_app* app, f32 dt) {
	app->graphics->api.clear_window(app->window, (iso_color) { 0, 0, 0, 1 });

	renderer2d_begin(renderer);
	renderer2d_push_mesh(renderer, mesh);
	renderer2d_end(renderer);
}

void iso_event(iso_app* app, SDL_Event event) {
	if (event.type == SDL_QUIT) {
		app->state = ISO_APP_CLOSED;
	}
}

void iso_exit(iso_app* app) {
	mesh2d_delete(mesh);
	renderer2d_delete(renderer);
}

int main(int argc, char** argv) {
	return iso_run(argc, argv);
}
