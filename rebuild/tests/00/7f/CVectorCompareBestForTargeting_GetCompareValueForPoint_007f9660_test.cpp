#include <cstdio>
#include <cmath>
// Behaviour model of CVectorCompare_BestForTargeting::GetCompareValueForPoint (0x007F9660): delta from the anchor, optional XY flatten, normalise, then value(sqrDist, dot).
struct V { float X, Y, Z; };
static float Value(V anchor, V line, bool xy, V p)
{
    V d = { p.X - anchor.X, p.Y - anchor.Y, p.Z - anchor.Z };
    if (xy) d.Z = 0.0f;
    float s = d.Y * d.Y; s += d.X * d.X; s += d.Z * d.Z;
    if (s > 1e-4f) { float r = 1.0f / (float)sqrt(s); d.X *= r; d.Y *= r; d.Z *= r; }
    return s * 1000.0f + (d.X * line.X + d.Y * line.Y + d.Z * line.Z);
}
int main()
{
    V a = { 0, 0, 0 }, l = { 1, 0, 0 }, p = { 3, 4, 5 };
    float v = Value(a, l, true, p);
    if (fabs(v - (25.0f * 1000.0f + 0.6f)) > 1e-2f) { std::printf("007f9660_TEST FAIL %f\n", v); return 1; }
    std::printf("007f9660_TEST PASS\n"); return 0;
}