#ifndef LINE3D_H
#define LINE3D_H

#include "point3d.h"

class Line3D {
public:
    Line3D(const Point3D& p1, const Point3D& p2);
    const Point3D& p1;
    const Point3D& p2;
    const int x1, x2, y1, y2;
    const double z1, z2;
    const int dx, dy;
    const double dz;
};

#endif // LINE3D_H
