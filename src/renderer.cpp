#include <vector>
#include <stdio.h>
#include <iostream>

#include "renderer.h"
#include "../third_party/lodepng/lodepng.h"
#include "random_float.h"

// Clamp double to min/max of 0/1
inline double clamp(double x){ return x<0 ? 0 : x>1 ? 1 : x; }
// Clamp to between 0-255
inline int toInt(double x){ return int(clamp(x)*255+.5); }


void Scene::add(Object *object) {
    m_objects.push_back( object );
}

ObjectIntersection Scene::intersect(const Ray &ray) {
    ObjectIntersection isct = ObjectIntersection();
    ObjectIntersection temp;
    long size = m_objects.size();

    for (int i=0; i<size; i++){
        temp = m_objects.at((unsigned)i)->get_intersection(ray);

        if (temp.hit) {
            if (isct.u == 0 || temp.u < isct.u){
                if (isct.m != nullptr && isct.m->temp) {
                    printf("isct m");
                    delete isct.m;
                }
                isct = temp;
            }
        }
    }
    return isct;
}

void free_isct(ObjectIntersection& isct){
    if (isct.m != nullptr && isct.m->temp) {
        delete isct.m;
    }
}

glm::vec3 Scene::trace_ray(const Ray &ray, int depth, unsigned short*Xi) {

    ObjectIntersection isct = intersect(ray);

    // If no hit, return world colour
    if (!isct.hit) return glm::vec3();


    if (isct.m->does_emit()){
        auto k =  isct.m->get_emission();
        free_isct(isct);
        return k;
    }


    glm::vec3 colour = isct.m->get_colour();


    // Calculate max reflection
    float p = colour.x>colour.y && colour.x>colour.z ? colour.x : colour.y>colour.z ? colour.y : colour.z;

    // Russian roulette termination.
    // If random number between 0 and 1 is > p, terminate and return hit object's emmission
    float rnd = rand_float();
    if (++depth>5){
        if (rnd<p*0.9) { // Multiply by 0.9 to avoid infinite loop with colours of 1.0
            colour=colour*(0.9f/p);
        }
        else {
            
            auto k = isct.m->get_emission();
            free_isct(isct);
            return k;
        }
    }

    glm::vec3 x = ray.origin + ray.direction * isct.u;
    Ray reflected = isct.m->get_reflected_ray(ray, x, isct.n, Xi);
    free_isct(isct);
    
    return colour * ( trace_ray(reflected, depth, Xi) );
}



Renderer::Renderer(Scene *scene, Camera *camera) {
    m_scene = scene;
    m_camera = camera;
    m_pixel_buffer = new glm::vec3[m_camera->get_width()*m_camera->get_height()];
}

void Renderer::render(int samples) {
    int width = m_camera->get_width();
    int height = m_camera->get_height();
    float samples_recp = 1./samples;

    // Main Loop
    #pragma omp parallel for schedule(dynamic, 1)       // OpenMP
    for (int y=0; y<height; y++){
        unsigned short Xi[3]={0,0,y*y*y};               // Stores seed for erand48

        fprintf(stderr, "\rRendering (%i samples): %.2f%% ",      // Prints
                samples, (double)y/height*100);                   // progress

        for (int x=0; x<width; x++){
            glm::vec3 col = glm::vec3();

            for (int a=0; a<samples; a++){
                Ray ray = m_camera->get_ray(x, y);
                col = col + m_scene->trace_ray(ray,0,Xi);
            }

            m_pixel_buffer[(y)*width + x] = col * samples_recp;
        }
    }
}

void Renderer::save_image(const char *file_path) {
    int width = m_camera->get_width();
    int height = m_camera->get_height();

    std::vector<unsigned char> pixel_buffer;

    int pixel_count = width*height;

    for (int i=0; i<pixel_count; i++) {
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].x));
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].y));
        pixel_buffer.push_back(toInt(m_pixel_buffer[i].z));
        pixel_buffer.push_back(255);
    }

    //Encode the image
    unsigned error = lodepng::encode(file_path, pixel_buffer, width, height);
    //if there's an error, display it
    if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

    pixel_buffer.clear();
}
