/*
*  Simple monte carlo path tracer in C++
*
*  Compilation:
*      $ mkdir build
*      $ cd build
*      $ cmake ..
*      $ make
*
*  Usage:
*      $ ./pathtracer <number of samples>
*/

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

#include "material.h"
#include "objects.h"
#include "camera.h"
#include "renderer.h"
#include <glm/gtx/rotate_vector.hpp>


int main(int argc, char *argv[]) {

    time_t start, stop;
    time(&start);               // Start execution timer
    int samples = 4;            // Default samples per pixel

    if (argc == 2) samples = atoi(argv[1]);

    Camera camera = Camera(glm::vec3(0, -5, 2.5), glm::vec3(0,0,1), 1280, 720);     // Create camera
    Scene scene = Scene();                                              // Create scene

    // Add objects to scene
    scene.add( dynamic_cast<Object*>(new Sphere(glm::vec3(0,0,-1000), 1000, new DiffuseMaterial(false), glm::mat3(1.0f))));
    scene.add( dynamic_cast<Object*>(new Sphere(glm::vec3(-1004,0,0), 1000, new DiffuseMaterial(false,glm::vec3(0.85,0.4,0.4)), glm::mat3(1.0f))));
    scene.add( dynamic_cast<Object*>(new Sphere(glm::vec3(1004,0,0), 1000, new DiffuseMaterial(false,glm::vec3(0.4,0.4,0.85)), glm::mat3(1.0f))) );
    scene.add( dynamic_cast<Object*>(new Sphere(glm::vec3(0,1006,0), 1000, new DiffuseMaterial(false),  glm::mat3(1.0f))));
    scene.add( dynamic_cast<Object*>(new Sphere(glm::vec3(0,0,110), 100, new SpecularMaterial(true, glm::vec3(1,1,1), glm::vec3(2.2,2.2,2.2)), glm::mat3(1.0f))));
    scene.add( dynamic_cast<Object*>(new Mesh(glm::vec3(0, 0, 0), "../obj/suzanne.obj", new DiffuseMaterial(false, glm::vec3(0.9, 0.9, 0.9)), glm::mat3(2.0f)  * glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1, 0, 0))))) );


    Renderer renderer = Renderer(&scene, &camera);  // Create renderer with our scene and camera
    renderer.render(samples);                       // Render image to pixel buffer
    renderer.save_image("render.png");              // Save image

    // Print duration information
    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff/3600;
    int mins = ((int)diff/60)-(hrs*60);
    int secs = (int)diff-(hrs*3600)-(mins*60);
    printf("\rRendering (%i samples): Complete!\nTime Taken: %i hrs, %i mins, %i secs\n\n", samples, hrs, mins, secs);
    return 0;
}