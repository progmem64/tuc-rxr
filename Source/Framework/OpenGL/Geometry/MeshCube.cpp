#include "MeshCube.h"

static const Mesh::Vertex VERTS[] = { 
	{ { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
	{ { 1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
	{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }, 
	{ { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },

	{ { 1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
	{ { 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f } },
	{ { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f } },
	{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } }, 

	{ { -1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
	{ { -1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f } },
	{ { 1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f } },
	{ { 1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } }, 

	{ { -1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
	{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
	{ { 1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f } },
	{ { -1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f } }, 

	{ { -1.0f, -1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f } },
	{ { -1.0f, -1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f } },
	{ { -1.0f, 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f } },
	{ { -1.0f, 1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f } }, 

	{ { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } },
	{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } },
	{ { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } },
	{ { 1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f } }, };

static const unsigned int INDICES[] = { 
	1, 2, 4,
	2, 3, 4,
	5, 6, 8,
	6, 7, 8,
	9, 10, 12,
	10, 11, 12,
	13, 14, 16,
	14, 15, 16,
	17, 18, 20,
	18, 19, 20,
	21, 22, 24,
	22, 23, 24};

MeshCube::MeshCube()
	:
	Mesh(VERTS,sizeof(VERTS) / sizeof(Vertex),INDICES, sizeof(INDICES) / sizeof(unsigned int))
{
}