// Behaviour test for 0x00BF7DF0 CLandscapeBackgroundTreeNode::GetDesiredLODBand()
#include <stdio.h>

struct V { float X, Y, Z; };
struct Box { V Min, Max; };
static float Clamp(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static float DistanceSquared(const Box& b, const V& p)   // both retail paths compute this (SSE max/min or x87 clamps)
{
    float dx = p.X - Clamp(p.X, b.Min.X, b.Max.X), dy = p.Y - Clamp(p.Y, b.Min.Y, b.Max.Y), dz = p.Z - Clamp(p.Z, b.Min.Z, b.Max.Z);
    return dx * dx + dy * dy + dz * dz;
}
static float g_bands[8] = { 0, 0, 10, 20, 40, 80, 160, 320 };
static int GetLODZBandAtZ(float z) { for (int band = 2; band < 8; ++band) if (z < g_bands[band]) return band - 1; return 7; }
static float Sqrt(float x) { float r = x > 1 ? x : 1; for (int i = 0; i < 40; ++i) r = 0.5f * (r + x / r); return x == 0 ? 0 : r; }
static int GetDesiredLODBand(const Box& b, const V& camera, int first, int last)   // the row's body
{
    int band = GetLODZBandAtZ(Sqrt(DistanceSquared(b, camera)));
    if (band < first) return first;
    if (band > last) return last;
    return band;
}

int main()
{
    Box b = { { 0, 0, 0 }, { 10, 10, 10 } };
    V inside = { 5, 5, 5 }, near = { 15, 5, 5 }, mid = { 40, 5, 5 }, far = { 5, 5, 1000 };
    int ok = 1;
    ok &= GetDesiredLODBand(b, inside, 0, 7) == 1;       // distance 0 < 10
    ok &= GetDesiredLODBand(b, near, 0, 7) == 1;         // 5
    ok &= GetDesiredLODBand(b, mid, 0, 7) == 3;          // 30: < 40 at band 4
    ok &= GetDesiredLODBand(b, far, 0, 7) == 7;          // past every band
    ok &= GetDesiredLODBand(b, far, 0, 5) == 5 && GetDesiredLODBand(b, inside, 3, 7) == 3;
    printf(ok ? "00bf7df0_TEST PASS\n" : "00bf7df0_TEST FAIL\n");
    return ok ? 0 : 1;
}