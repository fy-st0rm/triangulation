#include "triangulation.h"

f32 distance(iso_vec3 a, iso_vec3 b) {
	return sqrt((b.x - a.x) * (b.x - a.x) +
	            (b.y - a.y) * (b.y - a.y) +
	            (b.z - a.z) * (b.z - a.z));
}

b8 vec3_equals(iso_vec3 a, iso_vec3 b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

b8 f32_compare(f32 a, f32 b) {
	if (fabs(a - b) < 0.01f) {
		return true;
	}
	return false;
}

mesh2d* mesh2d_create(iso_vec3* vertices, u32 vertex_cnt, iso_color color) {
	mesh2d* mesh = iso_alloc(sizeof(mesh2d));

	mesh->vertices = iso_alloc(sizeof(iso_vec3) * vertex_cnt);
	memcpy(mesh->vertices, vertices, sizeof(iso_vec3) * vertex_cnt);
	mesh->vertex_cnt = vertex_cnt;
	mesh->color = color;

	return mesh;
}

void mesh2d_delete(mesh2d* mesh) {
	iso_free(mesh->vertices);
	iso_free(mesh);
}

void mesh2d_print(mesh2d* mesh) {
	for (i32 i = 0; i < mesh->vertex_cnt; i++) {
		iso_vec3 v = mesh->vertices[i];
		printf("%f %f %f\n", v.x, v.y, v.z);
	}
	printf("\n");
}

void mesh2d_sort_vertices(mesh2d* mesh) {
	for (i32 i = 0; i < mesh->vertex_cnt; i++) {
		for (i32 j = i + 1; j < mesh->vertex_cnt; j++) {
			if (mesh->vertices[i].x > mesh->vertices[j].x) {
				iso_vec3 k = mesh->vertices[j];
				mesh->vertices[j] = mesh->vertices[i];
				mesh->vertices[i] = k;
			}
		}
	}
}

i32 mesh2d_search(mesh2d* mesh, iso_vec3 vec) {
	for (i32 i = 0; i < mesh->vertex_cnt; i++) {
		iso_vec3 vertex = mesh->vertices[i];
		if (vec3_equals(vertex, vec))
			return i;
	}
	iso_assert(false, "Search failed\n");
	return -1;
}

b8 tri_equals(triangle tri_a, triangle tri_b) {
	b8 res1 = ( vec3_equals(tri_a.a, tri_b.a) &&
              vec3_equals(tri_a.b, tri_b.b) &&
              vec3_equals(tri_a.c, tri_b.c) );

	b8 res2 = ( vec3_equals(tri_a.a, tri_b.b) &&
              vec3_equals(tri_a.b, tri_b.c) &&
              vec3_equals(tri_a.c, tri_b.a) );

	b8 res3 = ( vec3_equals(tri_a.a, tri_b.c) &&
              vec3_equals(tri_a.b, tri_b.a) &&
              vec3_equals(tri_a.c, tri_b.b) );

	b8 res4 = ( vec3_equals(tri_a.c, tri_b.a) &&
              vec3_equals(tri_a.b, tri_b.b) &&
              vec3_equals(tri_a.a, tri_b.c) );

	b8 res5 = ( vec3_equals(tri_a.a, tri_b.b) &&
              vec3_equals(tri_a.b, tri_b.a) &&
              vec3_equals(tri_a.c, tri_b.c) );

	b8 res6 = ( vec3_equals(tri_a.a, tri_b.a) &&
              vec3_equals(tri_a.b, tri_b.c) &&
              vec3_equals(tri_a.c, tri_b.b) );

	return res1 || res2 || res3 || res4 || res5 || res6;
}

b8 line_intersect(iso_vec3 a1, iso_vec3 a2, iso_vec3 b1, iso_vec3 b2) {
	// Making 1 smaller then 2
	if (a1.x > a2.x) {
		iso_vec3 k = a2;
		a2 = a1;
		a1 = k;
	}

	if (b1.x > b2.x) {
		iso_vec3 k = b2;
		b2 = b1;
		b1 = k;
	}

	// Slopes
	f32 am = (a2.y - a1.y) / (a2.x - a1.x);
	f32 bm = (b2.y - b1.y) / (b2.x - b1.x);
	if (am == bm) return false;

	// Calculating intersect point of x
	f32 x = (am * a1.x - bm * b1.x - a1.y + b1.y) / (am - bm);

	// Returning if the intersecting point is one of the vertices
	if (f32_compare(x, a1.x) || f32_compare(x, a2.x) ||
      f32_compare(x, b1.x) || f32_compare(x, b2.x) ) {
		return false;
	}

	// If the point is in bound then itersect happened
	if (((a1.x < x) && (x < a2.x)) && ((b1.x < x) && (x < b2.x))) {
		return true;
	}
	return false;
}

b8 triangle_intersect(triangle tri_a, triangle tri_b) {
	b8 res = false;

	res = line_intersect(tri_a.a, tri_a.b, tri_b.a, tri_b.b) ||
        line_intersect(tri_a.a, tri_a.b, tri_b.a, tri_b.c) ||
        line_intersect(tri_a.a, tri_a.b, tri_b.b, tri_b.c) ||

        line_intersect(tri_a.b, tri_a.c, tri_b.a, tri_b.b) ||
        line_intersect(tri_a.b, tri_a.c, tri_b.a, tri_b.c) ||
        line_intersect(tri_a.b, tri_a.c, tri_b.b, tri_b.c) ||

        line_intersect(tri_a.a, tri_a.c, tri_b.a, tri_b.b) ||
        line_intersect(tri_a.a, tri_a.c, tri_b.a, tri_b.c) ||
        line_intersect(tri_a.a, tri_a.c, tri_b.b, tri_b.c);

	return res;
}

circle generate_circle_from_triangle(triangle tri) {
	iso_vec3 a = tri.a;
	iso_vec3 b = tri.b;
	iso_vec3 c = tri.c;

	// D = 2 [ Ax(Cy - By) + Bx(Ay - Cy) + Cx(By - Ay) ]
	f32 D = 2 * ( (a.x * ( c.y - b.y )) + 
                (b.y * ( a.y - c.y )) +
                (c.x * ( b.y - a.y )) );

	f32 A = (a.x * a.x) + (a.y * a.y);
	f32 B = (b.x * b.x) + (b.y * b.y);
	f32 C = (c.x * c.x) + (c.y * c.y);

	// Center points
	f32 x = 1/D * (( A * (c.y - b.y) ) + ( B * (a.y - c.y) ) + ( C * (b.y - a.y) ));
	f32 y = 1/D * (( A * (b.x - c.x) ) + ( B * (c.x - a.x) ) + ( C * (a.x - b.x) ));

	// Radius
	f32 r = sqrt( (x - a.x) * (x - a.x) + (y - a.y) * (y - a.y) );

	return (circle) {
		.center = { x, y, 0 },
		.radius = r
	};
}

void print_triangle(triangle tri) {
	printf("(%f %f %f) (%f %f %f) (%f %f %f)\n",
				tri.a.x, tri.a.y, tri.a.z,
				tri.b.x, tri.b.y, tri.b.z,
				tri.c.x, tri.c.y, tri.c.z);
}

void mesh2d_generate_indices(mesh2d* mesh) {
	triangle sup_tri = {
		{  0.0f,  5.0f, 0.0f },
		{ -5.0f, -5.0f, 0.0f },
		{  5.0f, -5.0f, 0.0f }
	};

	triangle bad_triangles[1000];
	i32 bad_tri_cnt = 0;

	// Adding super triangle
	bad_triangles[0] = sup_tri;
	bad_tri_cnt++;

	// Applying Watson Algorithm for trianulation
	for (i32 i = 0; i < mesh->vertex_cnt; i++) {
		iso_vec3 vertex = mesh->vertices[i];

		// Iterating through all the bad triangles
		i32 cnt = bad_tri_cnt;
		for (i32 j = 0; j < cnt; j++) {
			triangle tri = bad_triangles[j];
			circle circum_circle = generate_circle_from_triangle(tri);

			// Checking if the point lies in circum-circle
			f32 dist = distance(vertex, circum_circle.center);
			if (dist < circum_circle.radius) {

				// Creating new triangles
				bad_triangles[j] = (triangle) {
					.a = vertex,
					.b = tri.b,
					.c = tri.c
				};
				bad_triangles[bad_tri_cnt++] = (triangle) {
					.a = tri.a,
					.b = tri.b,
					.c = vertex 
				};
				bad_triangles[bad_tri_cnt++] = (triangle) {
					.a = tri.a,
					.b = vertex,
					.c = tri.c
				};

			}
		}

	}

	triangle good_triangles[1000];
	i32 good_tri_cnt = 0;

	// Removing super triangles
	for (i32 i = 0; i < bad_tri_cnt; i++) {
		triangle tri = bad_triangles[i];
		if (!(vec3_equals(tri.a, sup_tri.a) ||
          vec3_equals(tri.b, sup_tri.a) ||
          vec3_equals(tri.c, sup_tri.a) ||
          vec3_equals(tri.a, sup_tri.b) ||
          vec3_equals(tri.b, sup_tri.b) ||
          vec3_equals(tri.c, sup_tri.b) ||
          vec3_equals(tri.a, sup_tri.c) ||
          vec3_equals(tri.b, sup_tri.c) ||
          vec3_equals(tri.c, sup_tri.c))) {

			// Removing duplicate and intersecting triangles
			b8 res = false;
			for (i32 j = 0; j < good_tri_cnt; j++) {
				res = tri_equals(tri, good_triangles[j]) || triangle_intersect(tri, good_triangles[j]);
				if (res) break;
			}
			if (!res) {
				good_triangles[good_tri_cnt++] = tri;
			}
		}
	}

	// Generating indices
	u32 indices[good_tri_cnt * 3];
	i32 index_cnt = 0;
	for (i32 i = 0; i < good_tri_cnt; i++) {
		triangle tri = good_triangles[i];
		i32 idx_a = mesh2d_search(mesh, tri.a);
		i32 idx_b = mesh2d_search(mesh, tri.b);
		i32 idx_c = mesh2d_search(mesh, tri.c);
		indices[index_cnt++] = idx_a;
		indices[index_cnt++] = idx_b;
		indices[index_cnt++] = idx_c;
	}

	printf("----BAD TRIANGLES----\n");
	for (i32 i = 0; i < bad_tri_cnt; i++) {
		print_triangle(bad_triangles[i]);
	}
	printf("----BAD TRIANGLES----\n");

	printf("----GOOD TRIANGLES----\n");
	for (i32 i = 0; i < good_tri_cnt; i++) {
		print_triangle(good_triangles[i]);
	}
	printf("----GOOD TRIANGLES----\n");

	printf("----INDICES----\n");
	for (i32 i = 0, j = 0; i < index_cnt; i++) {
		printf("%d ", indices[i]);
		j++;
		if (j >= 3) {
			printf("\n");
			j = 0;
		}
	}
	printf("----INDICES----\n");
	printf("Bad triangles count: %d\n", bad_tri_cnt);
	printf("Triangles count: %d\n", good_tri_cnt);
}

