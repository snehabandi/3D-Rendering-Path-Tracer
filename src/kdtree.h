#ifndef KDTREE_H
#define KDTREE_H

#include <vector>

#include "ray.h"
#include "objects.h"

struct Triangle;

class KDNode {
public:
    AABB box;
    KDNode* left;
    KDNode* right;
    std::vector<Triangle*> triangles;
    bool leaf;

    KDNode(){};
    KDNode* build(std::vector<Triangle*> &tris, int depth);
    bool hit (KDNode* node, const Ray &ray, float &t, float &tmin, glm::vec3 &normal, glm::vec3 &c);
};

#endif // KDTREE_H