#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "../src/mesh_gen.h"
bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
);

mesh_data
loadAssImp(const char * path);

#endif
