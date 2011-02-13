#ifndef _GEOMETRY_HPP_
#define _GEOMETRY_HPP_

struct Triangle {
	Triangle() : a(-1), b(-1), c(-1) {}
	Triangle(int a, int b, int c) : a(a), b(b), c(c) {}
	union {
		struct {
			int a, b, c;
		};
		struct {
			int i[3];
		};
	};
};

struct Quad {
	Quad() : a(nullptr), b(nullptr), c(nullptr), d(nullptr) {}
	Quad(D3DXVECTOR3 *a, D3DXVECTOR3 *b, D3DXVECTOR3 *c, D3DXVECTOR3 *d) : a(a), b(b), c(c), d(d) {}
	D3DXVECTOR3 *a, *b, *c, *d;
};

typedef std::vector<D3DXVECTOR3> Vertices;
typedef std::vector<Triangle *> Triangles;

void create_cube(const D3DXVECTOR3 &center, const D3DXVECTOR3 &extents, Triangles *tris, Vertices *verts);
void create_sphere(std::vector<Triangle *> *tris, std::vector<D3DXVECTOR3> *verts);

#endif
