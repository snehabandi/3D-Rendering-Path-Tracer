#ifndef OBJECTS_H
#define OBJECTS_H

#include "aabb.h"
#include "kdtree.h"
#include "material.h"
#include "ray.h"

#include "../third_party/tiny_obj_loader/tiny_obj_loader.h"
//#include "../third_party/fastbvh/BVH.h"

struct Triangle {
    glm::vec3 v0, v1, v2;     // Vertex world space coordinates
    glm::vec3 e1, e2;         // Edge 1 and edge 2 of triangle
    glm::vec3 n, t0, t1, t2;  // Triangle normal and texture coordinates
    Material *m;        // Pointer to material

    Triangle(glm::vec3 v0_, glm::vec3 v1_, glm::vec3 v2_, glm::vec3 t0_=glm::vec3(), glm::vec3 t1_=glm::vec3(), glm::vec3 t2_=glm::vec3(), Material *m_=NULL){
        v0=v0_, v1=v1_, v2=v2_, e1=v1-v0, e2=v2-v0, n=glm::normalize(glm::cross(e1, e2));
        t0=t0_, t1=t1_, t2=t2_;
        m=m_;
    }

    // Returns axis aligned bounding box that contains the triangle
    AABB get_bounding_box(){
        glm::vec3 bl = glm::vec3(
                std::min (std::min(v0.x, v1.x), v2.x ) ,
                std::min (std::min(v0.y, v1.y), v2.y ) ,
                std::min (std::min(v0.z, v1.z), v2.z )
        );
        glm::vec3 tr = glm::vec3(
                std::max (std::max(v0.x, v1.x), v2.x ) ,
                std::max (std::max(v0.y, v1.y), v2.y ) ,
                std::max (std::max(v0.z, v1.z), v2.z )
        );

        return AABB(bl, tr);
    }

    // Returns the midpoint of the triangle
    glm::vec3 get_midpoint(){
        return (v0 + v1 + v2)/3.0f;
    }

    // Checks if ray intersects with triangle. Returns true/false.
    bool intersect(Ray ray, float &t, float tmin, glm::vec3 &norm) const {

        float u, v, t_temp=0;

        glm::vec3 pvec = glm::cross(ray.direction, e2);
        float det = glm::dot(e1, pvec);
        if (det == 0) return false;
        float invDet = 1. / det;
        glm::vec3 tvec = ray.origin - v0;
        u = glm::dot(tvec, pvec) * invDet;
        if (u < 0 || u > 1) return false;
        glm::vec3 qvec = glm::cross(tvec, e1);
        v = glm::dot(ray.direction, qvec) * invDet;
        if (v < 0 || u + v > 1) return false;
        t_temp = glm::dot(e2, (qvec)) * invDet; // Set distance along ray to intersection
        if (t_temp < tmin) {
            if (t_temp > 1e-9 ){    // Fairly arbritarily small value, scared to change
                t = t_temp;         // it as it works.
                norm = n;
                return true;
            }
        }
        return false;
    }

    // Returns barycentric coordinates of point p on the triangle
    glm::vec3 barycentric(glm::vec3 p){
        glm::vec3 v2_ = p - v0;
        float d00 = glm::dot(e1, e1);
        float d01 = glm::dot(e1, e2);
        float d11 = glm::dot(e2, e2);
        float d20 = glm::dot(v2_, e1);
        float d21 = glm::dot(v2_, e2);
        float d = d00*d11 - d01*d01;
        float v = (d11*d20 - d01*d21) / d;
        float w = (d00*d21 - d01*d20) / d;
        float u = 1 - v - w;
        return glm::vec3(u, v, w);
    }

    // Returns the colour at point p on the triangle
    glm::vec3 get_colour_at(glm::vec3 p){
        if(m==NULL) return glm::vec3(1,0,1);

        glm::vec3 b = barycentric(p);
        glm::vec3 c = glm::vec3();
        c = c + (t0 * b.x);
        c = c + (t1 * b.y);
        c = c + (t2 * b.z);

        return m->get_colour_at(c.x, c.y);
    }

};


struct ObjectIntersection {
	bool hit;	// If there was an intersection
	float u;	// Distance to intersection along ray
	glm::vec3 n;		// Normal of intersected face
	Material* m;	// Material of intersected face

	ObjectIntersection(bool hit_=false, float u_=0, glm::vec3 n_=glm::vec3(), Material* m_=nullptr):
	hit{hit_},
	u{u_},
	n{n_},
	m{m_}{};
};


class Object {

public:
	glm::vec3 position; // Position
	Material* material;	// Material
    glm::mat3 transform;
	Object(glm::vec3 p, Material* m, glm::mat3 t): position{p}, material{m}, transform{t}{};
	virtual ObjectIntersection get_intersection(const Ray &r) = 0;
};


class Sphere : public Object {

private:
	float m_r;	// Radius
	

public:
	Sphere(glm::vec3 p_, double r_, Material* m_, glm::mat3 t_): Object(p_, m_, t_), m_r(r_){};	
	virtual float get_radius();
	virtual Material* get_material();

	virtual ObjectIntersection get_intersection(const Ray &r) override;
};


struct KDNode;

class Mesh : public Object {

private:
    std::vector<Material*> materials;
    std::vector<Triangle*> tris;
    KDNode *node;

public:
	Mesh(glm::vec3 p_, const char* file_path, Material* m_, glm::mat3 t);
	virtual ObjectIntersection get_intersection(const Ray &r) override;

};



#endif // OBJECTS_H