#ifndef OBJECTS_H
#define OBJECTS_H

#include "vector.h"
#include "ray.h"
#include "triangle.h"
#include "kdtree.h"
#include "material.h"
#include "../lib/tiny_obj_loader/tiny_obj_loader.h"
//#include "../lib/fastbvh/BVH.h"

struct ObjectIntersection {
	bool hit;	// If there was an intersection
	double u;	// Distance to intersection along ray
	Vec n;		// Normal of intersected face
	Material* m;	// Material of intersected face

	ObjectIntersection(bool hit_=false, double u_=0, Vec n_=Vec(), Material* m_=nullptr):
	hit{hit_},
	u{u_},
	n{n_},
	m{m_}{};
};


class Object {

public:
	Vec position; // Position
	Material* material;	// Material
	Object(Vec p, Material* m): position{p}, material{m}{};
	virtual ObjectIntersection get_intersection(const Ray &r) = 0;
};


class Sphere : public Object {

private:
	double m_r;	// Radius
	

public:
	Sphere(Vec p_, double r_, Material* m_): Object(p_, m_), m_r(r_){};	
	virtual double get_radius();
	virtual Material* get_material();

	virtual ObjectIntersection get_intersection(const Ray &r);
};


class Mesh : public Object {

private:
    std::vector<Material*> materials;
    std::vector<Triangle*> tris;
    KDNode *node;

public:
	Mesh(Vec p_, const char* file_path, Material* m_);
	virtual ObjectIntersection get_intersection(const Ray &r);

};


#endif // OBJECTS_H