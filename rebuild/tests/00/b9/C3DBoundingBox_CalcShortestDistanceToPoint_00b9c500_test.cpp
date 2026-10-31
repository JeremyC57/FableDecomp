// Behaviour test for 0x00B9C500 C3DBoundingBox::CalcShortestDistanceToPoint(const C3DVector&) const
#include <stdio.h>
struct V { float X, Y, Z; };
struct Box { V Min, Max; };
static float Clamp(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static float DistanceSquared(const Box& b, const V& p)   // both retail paths (SSE max/min kernel, x87 clamps)
{
    float dx = p.X - Clamp(p.X, b.Min.X, b.Max.X), dy = p.Y - Clamp(p.Y, b.Min.Y, b.Max.Y), dz = p.Z - Clamp(p.Z, b.Min.Z, b.Max.Z);
    return dx * dx + dy * dy + dz * dz;
}
static float Sqrt(float x) { float r = x > 1 ? x : 1; for (int i = 0; i < 40; ++i) r = 0.5f * (r + x / r); return x == 0 ? 0 : r; }
static float Row(const Box& b, const V& p) { return Sqrt(DistanceSquared(b, p)); }   // the row's body
static int Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    Box b = { { 0, 0, 0 }, { 10, 10, 10 } };
    V inside = { 5, 5, 5 }, px = { 13, 5, 5 }, corner = { -3, -4, 5 }, diag = { 13, 14, 10 };
    int ok = 1;
    ok &= Near(Row(b, inside), 0);
    ok &= Near(DistanceSquared(b, px), 9) && Near(DistanceSquared(b, corner), 25) && Near(DistanceSquared(b, diag), 25);
    ok &= Near(Row(b, px), 3) && Near(Row(b, corner), 5);
    printf(ok ? "00b9c500_TEST PASS\n" : "00b9c500_TEST FAIL\n");
    return ok ? 0 : 1;
}