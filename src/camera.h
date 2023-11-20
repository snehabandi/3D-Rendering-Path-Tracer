#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class Camera {

private:
    int m_width;
    float m_width_recp;
    int m_height;
    float m_height_recp;
    float m_ratio;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_x_direction;
    glm::vec3 m_y_direction;

public:
    Camera(glm::vec3 position, glm::vec3 target, int width, int height);
    int get_width();
    int get_height();
    Ray get_ray(int x, int y);

};

#endif //CAMERA_H