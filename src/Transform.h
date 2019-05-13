#ifndef TRANSFORM_H
#define TRANSFORM_H
class Transform
{
  public:
    Transform();
    void translate(glm::vec3 d);
    void set_translate(glm::vec3 d);
    void scale(glm::vec3 s);
    void set_scale(glm::vec3 s);
    void rotate_x(float angle);
    void rotate_y(float angle);
    void rotate_z(float angle);
    void update();
    glm::mat4 _model;

  private:
    glm::mat4 _translate;
    glm::mat4 _rotate;
    glm::mat4 _scale;
    float dx, dy, dz;
    float sx, sy, sz;
    float rot_x_angle;
    float rot_y_angle;
    float rot_z_angle;
};
#endif
