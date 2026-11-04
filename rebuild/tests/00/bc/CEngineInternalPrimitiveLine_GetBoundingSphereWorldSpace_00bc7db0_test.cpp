// Behaviour test for CEngineInternalPrimitiveLine::GetBoundingSphereWorldSpace (0x00BC7DB0): a model of the row -- the sphere
// is centred on the line's midpoint with radius half the line length, and the call returns true.
#include <stdio.h>
#include <math.h>
struct V { float X, Y, Z; };
struct Sphere { V Center; float Radius; };
static bool Bound(const V& s, const V& e, Sphere& out)
{
    out.Center.X = (s.X + e.X) * 0.5f; out.Center.Y = (s.Y + e.Y) * 0.5f; out.Center.Z = (s.Z + e.Z) * 0.5f;
    V c = out.Center;
    float dx = c.X - s.X, dy = c.Y - s.Y, dz = c.Z - s.Z;
    out.Radius = (float)sqrt(dx * dx + dz * dz + dy * dy);
    return true;
}
int main()
{
    V s = { 0, 0, 0 }, e = { 6, 8, 0 };
    Sphere sp;
    int ok = Bound(s, e, sp) && fabs(sp.Center.X - 3) < 1e-5 && fabs(sp.Center.Y - 4) < 1e-5 && fabs(sp.Radius - 5) < 1e-5;
    V a = { 1, 1, 1 }, b = { 1, 1, 5 };
    ok = ok && Bound(a, b, sp) && fabs(sp.Center.Z - 3) < 1e-5 && fabs(sp.Radius - 2) < 1e-5;
    printf(ok ? "00bc7db0_TEST PASS\n" : "00bc7db0_TEST FAIL\n");
    return ok ? 0 : 1;
}