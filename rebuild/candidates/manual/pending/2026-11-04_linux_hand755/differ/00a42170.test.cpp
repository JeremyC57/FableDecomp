// Behaviour test for CPlane::Initialise (0x00A42170), g++ model of the parked source.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static V Sub(V a, V b) { V r = { a.X - b.X, a.Y - b.Y, a.Z - b.Z }; return r; }
static V Norm(V v) { float l = std::sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); V r = { v.X / l, v.Y / l, v.Z / l }; return r; }
static V Cross(V a, V b) { V r = { a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X }; return r; }
struct Plane { V Normal; float Distance; bool Initialise(V a, V b, V c) { V e1 = Norm(Sub(b, a)), e2 = Norm(Sub(c, a)); Normal = Norm(Cross(e1, e2)); Distance = Normal.X * a.X + Normal.Y * a.Y + Normal.Z * a.Z; return true; } };
int main()
{
    Plane p; V a = { 0, 0, 2 }, b = { 3, 0, 2 }, c = { 0, 5, 2 };
    int ok = p.Initialise(a, b, c) && std::fabs(p.Normal.Z - 1) < 1e-6f && std::fabs(p.Normal.X) < 1e-6f && std::fabs(p.Distance - 2) < 1e-6f;
    ok &= p.Initialise(a, c, b) && std::fabs(p.Normal.Z + 1) < 1e-6f && std::fabs(p.Distance + 2) < 1e-6f;
    std::printf(ok ? "00a42170_TEST PASS\n" : "00a42170_TEST FAIL\n");
    return ok ? 0 : 1;
}
