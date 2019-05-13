#include <stdio.h>
#include <stdlib.h>

#include "Node.h"
#include "Transform.h"
#include "mesh_gen.h"
#include "../common/shader.hpp"
#include "../common/objloader.hpp"

Node::Node(glm::mat4 projection, glm::mat4 view)
{
  trans = Transform();
  glm::mat4 m = trans._model;
  normal_to_world = glm::transpose(glm::inverse(view * m));
  children = std::vector<Node const*>();
};


Node::~Node()
{
  glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &normalbuffer);
  glDeleteProgram(programID);
}

void
Node::set_attributes(glm::vec3 d, int a)
{
  diffuse = d;
  ind_size = a;
}

mesh_data
Node::load_mesh(const char * path)
{
  return loadAssImp(path);
}

void
Node::load_shader(const char *v_path, const char *f_path)
{
  //programID = LoadShaders("../shaders/light.vertexshader", "../shaders/light.fragmentshader");
  programID = LoadShaders(v_path, f_path);

  //this segment is to print out the uniforms used in the current shader
  GLint i;
  GLint count;

  GLint size; // size of the variable
  GLenum type; // type of the variable (float, vec3 or mat4, etc)

  const GLsizei bufSize = 16; // maximum name length
  GLchar name[bufSize]; // variable name in GLSL
  GLsizei length; // name length

  glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
  //printf("Active Uniforms: %d\n", count);

  /*
  for (i = 0; i < count; i++)
  {
      glGetActiveUniform(programID, (GLuint)i, bufSize, &length, &size, &type, name);

      printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
  }
  */
}

void
Node::load_to_GPU(mesh_data mesh)
{
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size()*sizeof(glm::vec3), static_cast<GLvoid const*>(mesh.vertices.data()), GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.normals.size()*sizeof(glm::vec3), static_cast<GLvoid const*>(mesh.normals.data()), GL_STATIC_DRAW);

  glGenBuffers(1, &indexbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size()*sizeof(unsigned int), static_cast<GLvoid const*>(mesh.indices.data()), GL_STATIC_DRAW);

  glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.indices.size()*sizeof(glm::vec2), static_cast<GLvoid const*>(mesh.uvs.data()), GL_STATIC_DRAW);
};

void
Node::render(glm::mat4 projection,glm::mat4 view,glm::vec3 light_pos, glm::vec3 camera_pos, glm::mat4 world) const
{
  if(vertexbuffer == 0u) return;
  glUseProgram(programID);

  auto const normal_model_to_world = glm::transpose(glm::inverse(world));
  auto const WVP = projection * view * world;

  glUniformMatrix4fv(glGetUniformLocation(programID, "WVP"), 1, GL_FALSE, &WVP[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(programID, "model_to_world"), 1, GL_FALSE, &world[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(programID, "normal_to_world"), 1, GL_FALSE, &normal_model_to_world[0][0]);
  glUniform3fv(glGetUniformLocation(programID, "light_pos"), 1, &light_pos[0]);
  glUniform3fv(glGetUniformLocation(programID, "camera_pos"), 1, &camera_pos[0]);
  //additional uniforms
  glUniform3fv(glGetUniformLocation(programID, "diffuse"), 1, &diffuse[0]);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  //glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawElements(GL_TRIANGLES, ind_size, GL_UNSIGNED_INT, (void*)0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);

};

void
Node::update(glm::mat4 projection, glm::mat4 view)
{
  trans.update();
}

glm::mat4
Node::get_model() const
{
  return trans._model;
}

void
Node::add_child(Node const* child)
{
	if (child == nullptr)
		printf("Trying to add a nullptr as child!");
	children.push_back(child);
}

size_t
Node::get_children_nb() const
{
	return children.size();
}

Node const*
Node::get_child(size_t index) const
{
	assert(index < children.size());
	return children[index];
}
