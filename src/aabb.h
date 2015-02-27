#ifndef AABB_H
#define AABB_H

#include <cfloat>
#include <algorithm>

#include "vector.h"
#include "ray.h"

// Axis-aligned bounding box
struct AABB {
    Vec bl;     // Bottom left (min)
    Vec tr;     // Top right   (max)

    AABB (Vec bl_=Vec(), Vec tr_=Vec()){
        bl=bl_, tr=tr_;
    }

    // Expand to fit box
    void expand(const AABB &box) {
        bl.x = std::min(bl.x, box.bl.x);
        bl.y = std::min(bl.y, box.bl.y);
        bl.z = std::min(bl.z, box.bl.z);

        tr.x = std::max(tr.x, box.tr.x);
        tr.y = std::max(tr.y, box.tr.y);
        tr.z = std::max(tr.z, box.tr.z);
    }


    // Returns longest axis: 0, 1, 2 for x, y, z respectively
    int get_longest_axis() {
        Vec diff = tr - bl;
        if (diff.x > diff.y && diff.x > diff.z) return 0;
        if (diff.y > diff.x && diff.y > diff.z) return 1;
        return 2;
    }

    // Check if ray intersects with box. Returns true/false and stores distance in t
    bool intersection(const Ray &r, double &t) {
        double tx1 = (bl.x - r.origin.x)*r.direction_inv.x;
        double tx2 = (tr.x - r.origin.x)*r.direction_inv.x;

        double tmin = std::min(tx1, tx2);
        double tmax = std::max(tx1, tx2);

        double ty1 = (bl.y - r.origin.y)*r.direction_inv.y;
        double ty2 = (tr.y - r.origin.y)*r.direction_inv.y;

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        double tz1 = (bl.z - r.origin.z)*r.direction_inv.z;
        double tz2 = (tr.z - r.origin.z)*r.direction_inv.z;

        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));
        t = tmin;

        return tmax >= tmin;
    }
};

#endif // AABB_H