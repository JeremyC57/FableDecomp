// Behaviour test for 00a593f0 (NBBBMaths::CCylinder::GetFirstIntersectionWithTriangle): a standalone copy of the reconstructed setup --
// the triangle plane cuts the cylinder in an ellipse whose minor radius is the cylinder radius and whose major radius is radius / cos,
// with the plane centre on the axis at (plane_dist - n.bottom) / (cos * height); a plane parallel to the axis is rejected.
#include <cstdio>
#include <cmath>
struct V3 { float x, y, z; };
static float dot(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static bool Ellipse(V3 bottom, V3 top, float radius, V3 n, float plane_dist, float& major, float& t)
{
    V3 ax = { top.x - bottom.x, top.y - bottom.y, top.z - bottom.z };
    float h = std::sqrt(dot(ax, ax));
    float c = dot(n, ax) / h; if (c < -1) c = -1; else if (c > 1) c = 1;
    if (!(std::fabs(c) > 1e-4f)) return false;
    t = (plane_dist - dot(n, bottom)) / (c * h);
    major = radius / c;
    return true;
}
int main()
{
    int fails = 0; float major = 0, t = 0;
    V3 b = { 0, 0, 0 }, top = { 0, 0, 2 };
    V3 up = { 0, 0, 1 };
    if (!Ellipse(b, top, 0.5f, up, 1.0f, major, t) || major != 0.5f || t != 0.5f) ++fails;   // horizontal cut halfway up: a circle
    V3 side = { 1, 0, 0 };
    if (Ellipse(b, top, 0.5f, side, 0.0f, major, t)) ++fails;                                   // plane parallel to the axis
    float s = std::sqrt(0.5f); V3 tilt = { s, 0, s };
    if (!Ellipse(b, top, 0.5f, tilt, s, major, t) || std::fabs(major - 0.5f / s) > 1e-5f) ++fails;
    if (fails) { std::printf("00a593f0_TEST FAIL %d\n", fails); return 1; }
    std::printf("00a593f0_TEST PASS\n");
    return 0;
}
