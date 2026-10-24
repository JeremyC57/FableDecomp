// Behaviour test for 0x0075FF00 NShape::IsVectorBetween2OthersInThePlane(a, b, c): true when the normalised c is
// closer in direction to both normalised a and b than they are to each other (c inside the angle a-b).
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#endif
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }   // test only
    C3DVector& Normalise();                       // 0x00A14440
    const float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};

namespace NShape {
bool __fastcall IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c);

bool __fastcall IsVectorBetween2OthersInThePlane(const C3DVector& a, const C3DVector& b, const C3DVector& c)
{
    C3DVector na = a;
    na.Normalise();
    C3DVector nb = b;
    nb.Normalise();
    C3DVector nc = c;
    nc.Normalise();
    float ab = na.Dot(nb);
    return na.Dot(nc) > ab && nb.Dot(nc) > ab;
}
}

C3DVector& C3DVector::Normalise()
{
    float len = (float)sqrt(X * X + Y * Y + Z * Z);
    if (len > 0.0f) { X /= len; Y /= len; Z /= len; }
    return *this;
}

int main()
{
    int ok = 1;
    C3DVector a(1.0f, 0.0f, 0.0f), b(0.0f, 5.0f, 0.0f);
    if (!NShape::IsVectorBetween2OthersInThePlane(a, b, C3DVector(2.0f, 2.0f, 0.0f))) ok = 0;    // 45 degrees: between
    if (!NShape::IsVectorBetween2OthersInThePlane(a, b, C3DVector(3.0f, 1.0f, 0.0f))) ok = 0;    // nearer a, still between
    if (NShape::IsVectorBetween2OthersInThePlane(a, b, C3DVector(-1.0f, 1.0f, 0.0f))) ok = 0;    // outside past b
    if (NShape::IsVectorBetween2OthersInThePlane(a, b, C3DVector(1.0f, -0.2f, 0.0f))) ok = 0;    // outside past a
    if (NShape::IsVectorBetween2OthersInThePlane(a, b, C3DVector(-1.0f, -1.0f, 0.0f))) ok = 0;   // opposite
    // inputs are copied, not normalised in place
    if (a.X != 1.0f || b.Y != 5.0f) ok = 0;
    printf("0075ff00_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}