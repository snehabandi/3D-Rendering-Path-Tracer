#ifndef CAMERA_H
#define CAMERA_H

#include "../lib/rand48/erand48.h"
#include "vector.h"
#include "ray.h"

class Camera {

private:
    int m_width;
    double m_width_recp;
    int m_height;
    double m_height_recp;
    double m_ratio;
    Vec m_position;
    Vec m_direction;
    Vec m_x_direction;
    Vec m_y_direction;

public:
    Camera(Vec position, Vec target, int width, int height);
    int get_width();
    int get_height();
    Ray get_ray(int x, int y);

};

#endif //CAMERA_H