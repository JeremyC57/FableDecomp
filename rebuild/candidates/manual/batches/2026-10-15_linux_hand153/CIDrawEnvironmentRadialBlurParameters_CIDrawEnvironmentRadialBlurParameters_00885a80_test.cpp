// Behaviour test for 0x00885a80: CIDrawEnvironmentRadialBlurParameters copy ctor: memberwise copy.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct V3 { float X, Y, Z; V3() {} V3(const V3& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct Col { float R, G, B, A; };
struct P { V3 a, b; Col t, i; float l, f, r, tw; int bp, ap; P() {} P(const P& o) : a(o.a), b(o.b), t(o.t), i(o.i), l(o.l), f(o.f), r(o.r), tw(o.tw), bp(o.bp), ap(o.ap) {} };
static int run() { P p; p.a.X = 1; p.a.Y = 2; p.a.Z = 3; p.b.X = 4; p.b.Y = 5; p.b.Z = 6; p.t.R = 7; p.t.A = 8; p.i.G = 9; p.i.B = 10; p.l = 11; p.f = 12; p.r = 13; p.tw = 14; p.bp = 15; p.ap = 16;
  P q(p); CHECK(q.a.Z == 3 && q.b.X == 4 && q.t.A == 8 && q.i.B == 10 && q.tw == 14 && q.bp == 15 && q.ap == 16); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00885a80_TEST FAIL\n"); return 1; }
    std::printf("00885a80_TEST PASS\n");
    return 0;
}
