// Behaviour test for 0x00A11AE0 CPhysicsMeshObject::GetBoundingBoxCentre(): the row's arithmetic over a flat model (box min/max,
// a 3x4 matrix of rotation rows plus position); checks the identity, a pure translation, and a rotation about Z with translation.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
struct M { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43; };
static V Centre(const V& mn, const V& mx, const M& m)
{
    V c = { (mx.X + mn.X) * 0.5f, (mx.Y + mn.Y) * 0.5f, (mx.Z + mn.Z) * 0.5f };
    const float xx = c.X, yy = c.Y, zz = c.Z;
    c.X = m.E11*xx+(m.E21*yy+m.E31*zz)+m.E41;
    c.Y = m.E12*xx+(m.E22*yy+m.E32*zz)+m.E42;
    c.Z = m.E23*yy+(m.E13*xx+m.E33*zz)+m.E43;
    return c;
}
static bool Near(const V& a, float x, float y, float z) { return std::fabs(a.X-x) < 1e-5f && std::fabs(a.Y-y) < 1e-5f && std::fabs(a.Z-z) < 1e-5f; }
int main()
{
    V mn = { -1, 0, 2 }, mx = { 3, 4, 6 };
    M id = { 1,0,0, 0,1,0, 0,0,1, 0,0,0 };
    M tr = { 1,0,0, 0,1,0, 0,0,1, 10,20,30 };
    M rz = { 0,1,0, -1,0,0, 0,0,1, 1,1,1 };      // x axis -> +y, y axis -> -x
    bool ok = Near(Centre(mn, mx, id), 1, 2, 4) && Near(Centre(mn, mx, tr), 11, 22, 34) && Near(Centre(mn, mx, rz), -1, 2, 5);
    std::printf(ok ? "00a11ae0_TEST PASS\n" : "00a11ae0_TEST FAIL\n");
    return ok ? 0 : 1;
}