#include "line3d.h"

Line3D::Line3D(const Point3D& p1, const Point3D& p2):
    p1(p1),
    p2(p2),
    x1(p1.x),
    x2(p2.x),
    y1(p1.y),
    y2(p2.y),
    z1(p1.z),
    z2(p2.z),
    dx(p2.x - p1.x),
    dy(p2.y - p1.y),
    dz(p2.z - p1.z)
{

}
