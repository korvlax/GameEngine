#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mesh_gen.h"


mesh_data
mesh_gen::gen_triangle(float base, float height)
{
  mesh_data triangle;
  triangle.vertices = std::vector<glm::vec3>();
  triangle.normals = std::vector<glm::vec3>();

  glm::vec3 v_1 = glm::vec3(-base/2.0f, 0.0f, 0.0f);
	glm::vec3 v_2 = glm::vec3(base/2.0f, 0.0f, 0.0f);
	glm::vec3 v_3 = glm::vec3(0.0f, height, 0.0f);
  triangle.vertices.push_back(v_1);
  triangle.vertices.push_back(v_2);
  triangle.vertices.push_back(v_3);

  triangle.normals.push_back(glm::vec3(glm::cross(v_1,v_2)));
  triangle.normals.push_back(glm::vec3(glm::cross(v_2,v_3)));
  triangle.normals.push_back(glm::vec3(glm::cross(v_1,v_3)));

  triangle.indices.push_back(0);
  triangle.indices.push_back(1);
  triangle.indices.push_back(2);

  //dont know  what to put here
  triangle.uvs.push_back(glm::vec2(0,0));
  triangle.uvs.push_back(glm::vec2(0,0));
  triangle.uvs.push_back(glm::vec2(0,0));
  return triangle;;
}
