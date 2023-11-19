#include "ray.h"
#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, int width, int height) {
    m_width = width;
    m_width_recp = 1./m_width;
    m_height = height;
    m_height_recp = 1./m_height;
    m_ratio = (double)m_width/m_height;

    m_position = position;
    m_direction = glm::normalize(target - m_position);
    m_x_direction = glm::normalize(glm::cross(glm::vec3(0, 0, 1), (m_direction * -1.0f)));
    m_y_direction = glm::normalize(glm::cross(m_x_direction, m_direction));
}

int Camera::get_width() { return m_width; }
int Camera::get_height() { return m_height; }

// Returns ray from camera origin through pixel at x,y
Ray Camera::get_ray(int x, int y) {
    glm::vec3 pixel = m_position + m_direction*2.0f;
    pixel = pixel - m_x_direction*m_ratio + m_x_direction*((x * 2.0f * m_ratio)*m_width_recp);
    pixel = pixel + m_y_direction - m_y_direction*((y * 2.0f)*m_height_recp);

    return Ray(m_position, glm::normalize((pixel-m_position)));
}