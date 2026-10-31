// Behaviour test for 0x00B99BB0 CEngineInternalPrimitiveRepeatedStaticMesh::IsInsideFrustum(const CEngineCamera&, bool)
#include <stdio.h>
struct V { float X, Y, Z; };
struct Box { V Min, Max; };
struct Plane { V N; float D; };
static float Clamp(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static float DistanceSquared(const Box& b, const V& p)   // both retail paths (SSE max/min, x87 clamps)
{
    float dx = p.X - Clamp(p.X, b.Min.X, b.Max.X), dy = p.Y - Clamp(p.Y, b.Min.Y, b.Max.Y), dz = p.Z - Clamp(p.Z, b.Min.Z, b.Max.Z);
    return dz * dz + dx * dx + dy * dy;
}
static float Sqrt(float x) { float r = x > 1 ? x : 1; for (int i = 0; i < 40; ++i) r = 0.5f * (r + x / r); return x == 0 ? 0 : r; }
static bool IsInsideFrustum(const Box& b, float factor, float maxDraw, const V& cam, const Plane* planes, bool ignore)   // the row's body
{
    float maxDistance = factor * maxDraw;
    if (Sqrt(DistanceSquared(b, cam)) > maxDistance) return false;
    if (ignore) return true;
    for (int i = 0; i < 4; ++i) {
        const Plane& p = planes[i];
        float x = p.N.X > 0 ? b.Min.X : b.Max.X, y = p.N.Y > 0 ? b.Min.Y : b.Max.Y, z = p.N.Z > 0 ? b.Min.Z : b.Max.Z;
        if (x * p.N.X + y * p.N.Y + z * p.N.Z > p.D) return false;
    }
    return true;
}
int main()
{
    Box b = { { 0, 0, 0 }, { 10, 10, 10 } };
    V cam = { 5, 5, -5 }, far = { 5, 5, -500 };
    Plane in[4] = { { { 1, 0, 0 }, 100 }, { { -1, 0, 0 }, 100 }, { { 0, 1, 0 }, 100 }, { { 0, -1, 0 }, 100 } };
    Plane out[4] = { { { 1, 0, 0 }, -50 }, { { -1, 0, 0 }, 100 }, { { 0, 1, 0 }, 100 }, { { 0, -1, 0 }, 100 } };
    int ok = 1;
    ok &= IsInsideFrustum(b, 1.0f, 100.0f, cam, in, false);          // near, inside every plane
    ok &= !IsInsideFrustum(b, 1.0f, 100.0f, far, in, false);         // 495 > 100: distance culled
    ok &= !IsInsideFrustum(b, 0.01f, 100.0f, cam, in, true);         // 5 > 1: culled even when ignoring planes
    ok &= IsInsideFrustum(b, 1.0f, 100.0f, cam, out, true);          // planes skipped
    ok &= !IsInsideFrustum(b, 1.0f, 100.0f, cam, out, false);        // nearest corner 0 > -50: outside plane 0
    printf(ok ? "00b99bb0_TEST PASS\n" : "00b99bb0_TEST FAIL\n");
    return ok ? 0 : 1;
}