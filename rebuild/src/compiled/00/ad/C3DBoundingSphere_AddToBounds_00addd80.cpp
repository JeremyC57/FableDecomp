// C3DBoundingSphere::AddToBounds(const C3DVector& point, float radius) -- grow the sphere to enclose another sphere: adopt it when empty or when it swallows this one, else span both
// Leaf: AddToBounds
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator/(float s) const { float inv = 1.0f / s; return C3DVector(X * inv, Y * inv, Z * inv); }
    float Length() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

class C3DBoundingSphere {
public:
    C3DVector Center;   // +0x00
    float     Radius;   // +0x0c
    void AddToBounds(const C3DVector& point, float radius);
};

void C3DBoundingSphere::AddToBounds(const C3DVector& point, float radius)
{
    if (Radius == 0.0f) {
        Center = point;
        Radius = radius;
    } else if (radius != 0.0f) {
        C3DVector offset = Center - point;
        float distance = offset.Length();
        if (Radius < radius) {
            if (radius - Radius > distance) {
                Center = point;
                Radius = radius;
                return;
            }
        } else if (Radius - radius > distance) {
            return;
        }
        Center = (point + Center + offset * (Radius - radius) / distance) * 0.5f;
        Radius = (distance + radius + Radius) * 0.5f;
    }
}