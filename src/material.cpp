#include <stdlib.h>

#include "ray.h"
#include "material.h"

glm::vec3 Material::get_colour() const { return m_colour; }

// Get colour at UV coordinates u,v
glm::vec3 Material::get_colour_at(double u, double v) const {
    if (m_texture.is_loaded())
        return m_texture.get_pixel(u, v);

    return m_colour;
}
glm::vec3 Material::get_emission() const { return m_emission; }

Ray DiffuseMaterial::get_reflected_ray(const Ray &r, glm::vec3 &p, const glm::vec3 &n,	unsigned short *Xi) const {
	// Ideal diffuse reflection
    // Cosine weighted sampling
    glm::vec3 nl=glm::dot(n, r.direction)<0?n:n*-1.0f;
    float r1=2*M_PI*erand48(Xi), r2=erand48(Xi), r2s=sqrt(r2);
    
    glm::vec3 w=nl;
    glm::vec3 u_intermediate = fabs(w.x)>.1?glm::vec3(0,1, 0):glm::vec3(1);
    glm::vec3 u=glm::normalize(glm::cross(u_intermediate, w));
    glm::vec3 v = glm::cross(w, u);
    glm::vec3 d = glm::normalize((u*cosf(r1)*r2s + v*sinf(r1)*r2s + w*sqrtf(1-r2)));
    // U: x axis
    // V: y axis
    // W: z axis

    return Ray(p, d);

}

Ray SpecularMaterial::get_reflected_ray(const Ray &r, glm::vec3 &p, const glm::vec3 &n,	unsigned short *Xi) const {
	// Ideal specular reflection
	
    double roughness = 0.8;
    glm::vec3 reflected = r.direction - n * 2.0f * glm::dot(n, r.direction);
    reflected = glm::normalize(glm::vec3(
        reflected.x + (erand48(Xi)-0.5)*roughness,
        reflected.y + (erand48(Xi)-0.5)*roughness,
        reflected.z + (erand48(Xi)-0.5)*roughness
    ));

    return Ray(p, reflected);	
}

