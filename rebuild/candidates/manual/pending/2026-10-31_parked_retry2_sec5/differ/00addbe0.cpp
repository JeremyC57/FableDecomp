// C3DBoundingSphere::BuildFromSubSpheres(const C3DBoundingSphere* spheres, long count) -- fit an axis-aligned box around the sub-spheres, centre on it, then grow the radius to enclose every sub-sphere
// Leaf: BuildFromSubSpheres
// Grade: DIFFER(430v414) -- the min/max loop as a guarded do/while down-counter stays rolled (VC7.1 unrolls the counted
//   for by 4); 80 diff lines v 332; left: frame 0x24 vs 0x14 (centre temporaries), compare scheduling
#include <math.h>

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float Length() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

class C3DBoundingSphere {
public:
    C3DVector Center;   // +0x00
    float     Radius;   // +0x0c
    void BuildFromSubSpheres(const C3DBoundingSphere* spheres, long count);
};

void C3DBoundingSphere::BuildFromSubSpheres(const C3DBoundingSphere* spheres, long count)
{
    C3DVector minimum(1e30f, 1e30f, 1e30f);
    C3DVector maximum(-1e30f, -1e30f, -1e30f);
    const C3DBoundingSphere* s = spheres;
    long i;
    if (count > 0) {
    i = count;
    do {
        if (minimum.X > s->Center.X - s->Radius) minimum.X = s->Center.X - s->Radius;
        if (minimum.Y > s->Center.Y - s->Radius) minimum.Y = s->Center.Y - s->Radius;
        if (minimum.Z > s->Center.Z - s->Radius) minimum.Z = s->Center.Z - s->Radius;
        if (maximum.X < s->Center.X + s->Radius) maximum.X = s->Center.X + s->Radius;
        if (maximum.Y < s->Center.Y + s->Radius) maximum.Y = s->Center.Y + s->Radius;
        if (maximum.Z < s->Center.Z + s->Radius) maximum.Z = s->Center.Z + s->Radius;
        ++s;
    } while (--i);
    }
    Radius = 0.0f;
    C3DVector centre = (maximum + minimum) * 0.5f;
    Center = centre;
    s = spheres;
    for (i = 0; i < count; ++i, ++s) {
        float reach = (s->Center - centre).Length() + s->Radius;
        if (reach > Radius) Radius = reach;
    }
}
