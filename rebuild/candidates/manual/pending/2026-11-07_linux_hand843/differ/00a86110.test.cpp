// Behaviour test for CIPhysicsFacesSegmentBase::CalculateDividingPlane (0x00A86110): two faces, each axis.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Tri { unsigned short I[3]; };
struct Plane { V Normal; float Distance; };
static const V Axes[3] = { V(0, 0, 1), V(1, 0, 0), V(0, 1, 0) };
static Plane Calc(const V* v, const Tri* t, const unsigned short* f, long count, long axis)
{
    V mn = v[t[f[0]].I[0]], mx = mn; V n = Axes[axis];
    for (long i = 0; i < count; ++i) {
        const Tri& tr = t[f[i]]; V p[3] = { v[tr.I[0]], v[tr.I[1]], v[tr.I[2]] };
        for (const V* q = p; q < p + 3; ++q) {
            if (q->X < mn.X) mn.X = q->X; if (q->Y < mn.Y) mn.Y = q->Y; if (q->Z < mn.Z) mn.Z = q->Z;
            if (q->X > mx.X) mx.X = q->X; if (q->Y > mx.Y) mx.Y = q->Y; if (q->Z > mx.Z) mx.Z = q->Z;
        }
    }
    V c((mn.X + mx.X) * 0.5f, (mn.Y + mx.Y) * 0.5f, (mn.Z + mx.Z) * 0.5f); V s(0, 0, 0);
    for (long i = 0; i < count; ++i) {
        const Tri& tr = t[f[i]]; V a = v[tr.I[1]], b = v[tr.I[2]];
        a.X -= c.X; a.Y -= c.Y; a.Z -= c.Z; b.X -= c.X; b.Y -= c.Y; b.Z -= c.Z;
        s.X += (b.X + a.X + b.X) * 0.333333f; s.Y += (b.Y + a.Y + b.Y) * 0.333333f; s.Z += (b.Z + a.Z + b.Z) * 0.333333f;
    }
    float k = 1.0f / count; s.X = s.X * k + c.X; s.Y = s.Y * k + c.Y; s.Z = s.Z * k + c.Z;
    Plane p; p.Normal = n; p.Distance = s.X * n.X + s.Y * n.Y + s.Z * n.Z; return p;
}
int main()
{
    V v[6] = { V(0, 0, 0), V(2, 0, 0), V(0, 4, 0), V(10, 0, 6), V(12, 0, 6), V(10, 4, 6) };
    Tri t[2] = { { { 0, 1, 2 } }, { { 3, 4, 5 } } };
    unsigned short f[2] = { 0, 1 };
    // centre (6,2,3); face 0: a=(2,0,0)-c=(-4,-2,-3), b=(0,4,0)-c=(-6,2,-3) -> (b+a+b)/3 = (-16,2,-9)/3
    // face 1: a=(12,0,6)-c=(6,-2,3), b=(10,4,6)-c=(4,2,3) -> (14,2,9)/3; mean = (-2,4,0)/3/2 = (-1/3,2/3,0) + c
    Plane z = Calc(v, t, f, 2, 0), x = Calc(v, t, f, 2, 1), y = Calc(v, t, f, 2, 2);
    if (std::fabs(z.Distance - 3.0f) > 1e-3f || std::fabs(x.Distance - (6.0f - 1.0f / 3)) > 1e-3f || std::fabs(y.Distance - (2.0f + 2.0f / 3)) > 1e-3f
        || x.Normal.X != 1 || y.Normal.Y != 1 || z.Normal.Z != 1) { std::printf("00a86110_TEST FAIL\n"); return 1; }
    std::printf("00a86110_TEST PASS\n");
    return 0;
}
