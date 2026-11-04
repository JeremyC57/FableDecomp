// Behaviour test for CPhysicsMeshObject::GetBoundingBoxXEdge (0x00A11C20): a model of the row -- half the box's X extent as
// (h, 0, 0), rotated in place by the matrix's 3x3 part (x*E1k + y*E2k + z*E3k), translation ignored.
#include <stdio.h>
#include <math.h>
struct V { float X, Y, Z; };
struct M { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43; };
static V XEdge(const V& mn, const V& mx, const M& m)
{
    V v = { (mx.X - mn.X) * 0.5f, 0.0f, 0.0f };
    const float xx = v.X, yy = v.Y, zz = v.Z;
    v.X = xx * m.E11 + yy * m.E21 + zz * m.E31;
    v.Y = xx * m.E12 + yy * m.E22 + zz * m.E32;
    v.Z = xx * m.E13 + yy * m.E23 + zz * m.E33;
    return v;
}
int main()
{
    V mn = { -2, -1, -1 }, mx = { 4, 1, 1 };
    M id = { 1,0,0, 0,1,0, 0,0,1, 10,20,30 };
    V a = XEdge(mn, mx, id);
    int ok = fabs(a.X - 3) < 1e-6 && a.Y == 0 && a.Z == 0;
    M rz = { 0,1,0, -1,0,0, 0,0,1, 0,0,0 };   // 90 degrees about Z: X axis -> Y
    V b = XEdge(mn, mx, rz);
    ok = ok && fabs(b.X) < 1e-6 && fabs(b.Y - 3) < 1e-6 && b.Z == 0;
    printf(ok ? "00a11c20_TEST PASS\n" : "00a11c20_TEST FAIL\n");
    return ok ? 0 : 1;
}