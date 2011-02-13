#include "stdafx.h"
#include "geometry.hpp"

void create_cube(const D3DXVECTOR3 &center, const D3DXVECTOR3 &extents, Triangles *tris, Vertices *verts)
{
	float ex = extents.x / 2;
	float ey = extents.y / 2;
	float ez = extents.z / 2;

	int ofs = verts->size();
	verts->push_back(D3DXVECTOR3(center + ex * +kDirX - ey * kDirY - ez * kDirZ));
	verts->push_back(D3DXVECTOR3(center - ex * +kDirX - ey * kDirY - ez * kDirZ));
	verts->push_back(D3DXVECTOR3(center - ex * +kDirX + ey * kDirY - ez * kDirZ));
	verts->push_back(D3DXVECTOR3(center + ex * +kDirX + ey * kDirY - ez * kDirZ));

	verts->push_back(D3DXVECTOR3(center + ex * +kDirX - ey * kDirY + ez * kDirZ));
	verts->push_back(D3DXVECTOR3(center - ex * +kDirX - ey * kDirY + ez * kDirZ));
	verts->push_back(D3DXVECTOR3(center - ex * +kDirX + ey * kDirY + ez * kDirZ));
	verts->push_back(D3DXVECTOR3(center + ex * +kDirX + ey * kDirY + ez * kDirZ));

#define ADD_TRI(a, b, c) tris->push_back(new Triangle(ofs + a, ofs + b, ofs + c))

#define ADD_QUAD(a, b, c, d) \
	ADD_TRI(a, b, c); ADD_TRI(a, c, d);

	ADD_QUAD(0, 1, 2, 3);
	ADD_QUAD(2, 6, 7, 3);
	ADD_QUAD(4, 7, 6, 5);
	ADD_QUAD(3, 2, 6, 7);
	ADD_QUAD(4, 0, 3, 7);
	ADD_QUAD(1, 5, 6, 2);
}
