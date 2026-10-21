// Behaviour test for CTCPhysicsStandard::GetRHSet (0x00723c80): the interpolated set is unit length and points along the lerp.
#include <cstdio>
#include <cmath>
#ifndef _MSC_VER
#define __forceinline inline
#endif
unsigned int g_InvSqrtMantissaTable[128];
static void BuildTable()
{
    // index = exponent LSB (bit 6) + top six mantissa bits; estimate exponent is (0x17C - e) / 2 = 126 for e = 127/128
    for (int i = 0; i < 128; ++i) {
        double x = (1.0 + ((i & 63) + 0.5) / 64.0) * ((i & 64) ? 1.0 : 2.0);
        double m = (2.0 / std::sqrt(x) - 1.0) * 256.0 + 0.5;
        if (m < 0.0) m = 0.0;
        if (m > 255.0) m = 255.0;
        g_InvSqrtMantissaTable[i] = (unsigned int)m;
    }
}
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    __forceinline void Normalise()
    {
        float lengthSquared = Z * Z;
        lengthSquared += Y * Y;
        lengthSquared += X * X;
        unsigned int bits = *(unsigned int*)&lengthSquared;
        unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r;
        r *= 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
struct CRightHandedSet {
    C3DVector Up;           // +0x00
    C3DVector Forward;      // +0x0c
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
struct CTCPhysicsStandard {
    unsigned char _pad[0x50];
    CRightHandedSet RHSet;      // +0x50
    CRightHandedSet OldRHSet;   // +0x68
    CRightHandedSet GetRHSet(float t) const;
};

CRightHandedSet CTCPhysicsStandard::GetRHSet(float t) const
{
    C3DVector up = (RHSet.Up - OldRHSet.Up) * t + OldRHSet.Up;
    C3DVector forward = (RHSet.Forward - OldRHSet.Forward) * t + OldRHSet.Forward;
    up.Normalise();
    forward.Normalise();
    return CRightHandedSet(up, forward);
}

static bool Near(float a, float b) { return std::fabs(a - b) < 2e-3f; }
static C3DVector V(float x, float y, float z) { return C3DVector(x, y, z); }
int main()
{
    BuildTable();
    CTCPhysicsStandard p;
    p.OldRHSet.Up = V(0.0f, 0.0f, 2.0f);
    p.RHSet.Up = V(0.0f, 4.0f, 2.0f);
    p.OldRHSet.Forward = V(3.0f, 0.0f, 0.0f);
    p.RHSet.Forward = V(3.0f, 0.0f, 0.0f);
    CRightHandedSet s = p.GetRHSet(0.5f);
    // up lerps to (0,2,2) -> (0, .7071, .7071); forward stays (3,0,0) -> (1,0,0)
    if (!Near(s.Up.X, 0.0f) || !Near(s.Up.Y, 0.70710678f) || !Near(s.Up.Z, 0.70710678f) ||
        !Near(s.Forward.X, 1.0f) || !Near(s.Forward.Y, 0.0f) || !Near(s.Forward.Z, 0.0f)) {
        std::printf("00723c80_TEST FAIL\n"); return 1;
    }
    s = p.GetRHSet(1.0f);   // t = 1 gives the current set: up (0,4,2) -> (0, .8944, .4472)
    if (!Near(s.Up.Y, 0.89442719f) || !Near(s.Up.Z, 0.44721360f) || !Near(s.Forward.X, 1.0f)) {
        std::printf("00723c80_TEST FAIL\n"); return 1;
    }
    s = p.GetRHSet(0.0f);   // t = 0 gives the old set
    if (!Near(s.Up.Z, 1.0f) || !Near(s.Up.Y, 0.0f)) { std::printf("00723c80_TEST FAIL\n"); return 1; }
    std::printf("00723c80_TEST PASS\n");
    return 0;
}