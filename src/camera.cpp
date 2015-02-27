#include "ray.h"
#include "camera.h"

Camera::Camera(Vec position, Vec target, int width, int height) {
    m_width = width;
    m_width_recp = 1./m_width;
    m_height = height;
    m_height_recp = 1./m_height;
    m_ratio = (double)m_width/m_height;

    m_position = position;
    m_direction = (target - m_position).norm();
    m_x_direction = Vec(0, 0, 1).cross(m_direction * -1).norm();
    m_y_direction = m_x_direction.cross(m_direction).norm();
}

int Camera::get_width() { return m_width; }
int Camera::get_height() { return m_height; }

// Returns ray from camera origin through pixel at x,y
Ray Camera::get_ray(int x, int y) {
    Vec pixel = m_position + m_direction*2;
    pixel = pixel - m_x_direction*m_ratio + m_x_direction*((x * 2 * m_ratio)*m_width_recp);
    pixel = pixel + m_y_direction - m_y_direction*((y * 2.0)*m_height_recp);

    return Ray(m_position, (pixel-m_position).norm());
}