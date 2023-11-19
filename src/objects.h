#ifndef OBJECTS_H
#define OBJECTS_H

#include "ray.h"
#include "triangle.h"
#include "kdtree.h"
#include "material.h"
#include "../lib/tiny_obj_loader/tiny_obj_loader.h"
//#include "../lib/fastbvh/BVH.h"

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
	Object(glm::vec3 p, Material* m): position{p}, material{m}{};
	virtual ObjectIntersection get_intersection(const Ray &r) = 0;
};


class Sphere : public Object {

private:
	float m_r;	// Radius
	

public:
	Sphere(glm::vec3 p_, double r_, Material* m_): Object(p_, m_), m_r(r_){};	
	virtual float get_radius();
	virtual Material* get_material();

	virtual ObjectIntersection get_intersection(const Ray &r) override;
};


class Mesh : public Object {

private:
    std::vector<Material*> materials;
    std::vector<Triangle*> tris;
    KDNode *node;

public:
	Mesh(glm::vec3 p_, const char* file_path, Material* m_);
	virtual ObjectIntersection get_intersection(const Ray &r) override;

};


#endif // OBJECTS_H