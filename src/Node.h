#ifndef NODE_H
#define NODE_H

#include "mesh_gen.h"
#include "Transform.h"
#include <GL/glew.h>
class Node
{
  public:
    Node(glm::mat4 projection, glm::mat4 view);
    ~Node();
    void set_normal_matrix(glm::mat4 m);
    void set_attributes(glm::vec3 d, int ind_size);
    mesh_data load_mesh(const char * path);
    void load_shader(const char *v_path, const char *f_path);
    void load_to_GPU(mesh_data mesh);
    void render(glm::mat4 projection, glm::mat4 view, glm::vec3 light_pos, glm::vec3 camera_pos, glm::mat4) const;
    void update(glm::mat4 projection, glm::mat4 view);
    glm::mat4 get_model() const;
    void add_child(Node const* child);
    size_t get_children_nb() const;
    Node const* get_child(size_t index) const;

    Transform trans;

  private:
    glm::mat4 normal_to_world;
    glm::vec3 diffuse;

    int ind_size;
    std::vector<Node const*> children;

    GLuint programID;
    GLuint vertexbuffer;
    GLuint normalbuffer;
    GLuint indexbuffer;
    GLuint uvbuffer;
};
#endif
