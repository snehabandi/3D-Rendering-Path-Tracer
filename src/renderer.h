#ifndef RENDERER_H
#define RENDERER_H

#include "camera.h"
#include "objects.h"
#include <glm/glm.hpp>


class Scene {
private:
    std::vector<Object*> m_objects;

public:
    Scene(){};
    void add(Object *object);
    ObjectIntersection intersect(const Ray &ray);
    glm::vec3 trace_ray(const Ray &ray, int depth, unsigned short*Xi);
};


class Renderer {

private:
    Scene *m_scene;
    Camera *m_camera;
    glm::vec3 *m_pixel_buffer;

public:
    Renderer(Scene *scene, Camera *camera);
    void render(int samples=4);
    void save_image(const char * file_path);

};

#endif //RENDERER_H