#ifndef MESH_GEN_H
#define MESH_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct mesh_data{
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;
  std::vector<glm::vec2> uvs;
};
class mesh_gen
{
  public:
    mesh_data gen_triangle(float base, float height);
};
#endif
