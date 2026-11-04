// Behaviour test for NHitLocations::CPhysicalPrimitiveSphere::GetClosestPointToPos2D (0x00814EA0), g++ model of the parked source.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static const float ZERO = 1e-5f;
static V Closest(V centre, float radius, float thingRadius, bool creature, float combatRadius, V pos, V& dir)
{
    if (radius < ZERO) { radius = thingRadius; if (creature) radius = combatRadius; }
    V off = { pos.X - centre.X, pos.Y - centre.Y, 0.0f };
    float sq = off.X * off.X + off.Y * off.Y;
    if (sq > ZERO * ZERO) { float l = std::sqrt(sq); off.X /= l; off.Y /= l; }
    dir = off;
    V r = { off.X * radius + centre.X, off.Y * radius + centre.Y, off.Z * radius + centre.Z };
    return r;
}
int main()
{
    V c = { 1, 1, 5 }, p = { 4, 5, 0 }, d;
    V r = Closest(c, 2.0f, 9, false, 9, p, d);
    int ok = std::fabs(r.X - 2.2f) < 1e-5f && std::fabs(r.Y - 2.6f) < 1e-5f && r.Z == 5 && std::fabs(d.X - 0.6f) < 1e-6f && d.Z == 0;
    r = Closest(c, 0.0f, 3, true, 1.0f, p, d); ok &= std::fabs(r.X - 1.6f) < 1e-5f;
    r = Closest(c, 0.0f, 3, false, 1.0f, p, d); ok &= std::fabs(r.X - 2.8f) < 1e-5f;
    r = Closest(c, 2.0f, 9, false, 9, c, d); ok &= r.X == 1 && r.Y == 1 && d.X == 0;
    std::printf(ok ? "00814ea0_TEST PASS\n" : "00814ea0_TEST FAIL\n");
    return ok ? 0 : 1;
}
