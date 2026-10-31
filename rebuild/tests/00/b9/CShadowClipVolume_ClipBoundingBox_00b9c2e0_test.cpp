// Behaviour test for 0x00B9C2E0 CShadowClipVolume::ClipBoundingBox(const C3DBoundingBox&) const
#include <stdio.h>
struct V { float X, Y, Z; };
struct Box { V Min, Max; };
struct Plane { V N; float D; };
struct Volume { Plane Planes[8]; int Count; V Centre; float Radius; bool ClipToSphere; };
static float Clamp(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }
static float DistanceSquared(const Box& b, const V& p)
{
    float dx = p.X - Clamp(p.X, b.Min.X, b.Max.X), dy = p.Y - Clamp(p.Y, b.Min.Y, b.Max.Y), dz = p.Z - Clamp(p.Z, b.Min.Z, b.Max.Z);
    return dz * dz + dx * dx + dy * dy;
}
static bool ClipBoundingBox(const Volume& v, const Box& b)   // the row's body
{
    if (v.ClipToSphere && DistanceSquared(b, v.Centre) > v.Radius * v.Radius) return false;
    for (int i = 0; i < v.Count; ++i) {
        const Plane& p = v.Planes[i];
        float x = p.N.X > 0 ? b.Min.X : b.Max.X, y = p.N.Y > 0 ? b.Min.Y : b.Max.Y, z = p.N.Z > 0 ? b.Min.Z : b.Max.Z;
        if (x * p.N.X + y * p.N.Y + z * p.N.Z > p.D) return false;
    }
    return true;
}
int main()
{
    Box b = { { 0, 0, 0 }, { 10, 10, 10 } };
    Volume v = {};
    v.Count = 1; v.Planes[0].N.X = 1; v.Planes[0].D = 5;              // nearest corner x 0 <= 5: kept
    v.Centre.X = 20; v.Centre.Y = 5; v.Centre.Z = 5; v.Radius = 11;   // distance 10 < 11
    int ok = 1;
    ok &= ClipBoundingBox(v, b);
    v.ClipToSphere = true;
    ok &= ClipBoundingBox(v, b);
    v.Radius = 9;                                                     // 10 > 9: culled by the sphere
    ok &= !ClipBoundingBox(v, b);
    v.ClipToSphere = false; v.Planes[0].D = -1;                       // 0 > -1: culled by the plane
    ok &= !ClipBoundingBox(v, b);
    v.Count = 0;
    ok &= ClipBoundingBox(v, b);
    printf(ok ? "00b9c2e0_TEST PASS\n" : "00b9c2e0_TEST FAIL\n");
    return ok ? 0 : 1;
}