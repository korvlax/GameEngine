#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

Transform::Transform()
{
  dx = 0;
  dy = 0;
  dz = 0;
  rot_x_angle = 0.0f;
  rot_y_angle = 0.0f;
  rot_z_angle = 0.0f;
  sx = 1;
	sy = 1;
	sz = 1;
  update();
}

void Transform::update()
{
  _translate = glm::mat4(
												glm::vec4(1,0,0,0),
												glm::vec4(0,1,0,0),
												glm::vec4(0,0,1,0),
												glm::vec4(dx,dy,dz,1)
												);
  _scale = glm::mat4(
  											glm::vec4(sx,0,0,0),
  											glm::vec4(0,sy,0,0),
  											glm::vec4(0,0,sz,0),
  											glm::vec4(0,0,0,1)
  											);

  glm::mat4 rot_x = glm::mat4(
                        glm::vec4(1,0,0,0),
                        glm::vec4(0,glm::cos(rot_x_angle),glm::sin(rot_x_angle),0),
                        glm::vec4(0,-glm::sin(rot_x_angle),glm::cos(rot_x_angle),0),
                        glm::vec4(0,0,0,1)
                        );
  glm::mat4 rot_y = glm::mat4(
												glm::vec4(glm::cos(rot_y_angle),0,-glm::sin(rot_y_angle),0),
												glm::vec4(0,1,0,0),
												glm::vec4(glm::sin(rot_y_angle),0,glm::cos(rot_y_angle),0),
												glm::vec4(0,0,0,1)
												);
  glm::mat4 rot_z = glm::mat4(
                        glm::vec4(glm::cos(rot_z_angle),glm::sin(rot_z_angle),0,0),
                        glm::vec4(-glm::sin(rot_z_angle),glm::cos(rot_z_angle),0,0),
                        glm::vec4(0,0,1,0),
                        glm::vec4(0,0,0,1)
                        );
  _rotate = rot_x * rot_y * rot_z;

  _model = _translate * _rotate * _scale;
}

void Transform::translate(glm::vec3 d)
{
  dx += d.x;
  dy += d.y;
  dz += d.y;
}
void Transform::set_translate(glm::vec3 d)
{
  dx = d.x;
  dy = d.y;
  dz = d.y;
}

void Transform::scale(glm::vec3 s)
{
  sx += s.x;
  sy += s.y;
  sz += s.z;
}
void Transform::set_scale(glm::vec3 s)
{
  sx = s.x;
  sy = s.y;
  sz = s.z;
}
void Transform::rotate_x(float angle)
{
  rot_x_angle += angle;
}
void Transform::rotate_y(float angle)
{
  rot_y_angle += angle;
}
void Transform::rotate_z(float angle)
{
  rot_z_angle += angle;
}
