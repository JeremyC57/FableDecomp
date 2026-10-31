// Behaviour test for 0x00B4CED0 CShadowClipVolume::ClipBoundingBox(const C3DBoundingBox&, bool&) const
#include <stdio.h>
#include <math.h>
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
static bool Clip(const Volume& v, const Box& b, bool& inside)   // the row's body
{
    inside = true;
    if (v.ClipToSphere) {
        const V& c = v.Centre;
        if (DistanceSquared(b, c) > v.Radius * v.Radius) { inside = false; return false; }
        float fx = fabsf(c.X - b.Min.X) > fabsf(c.X - b.Max.X) ? b.Min.X : b.Max.X;
        float fy = fabsf(c.Y - b.Min.Y) > fabsf(c.Y - b.Max.Y) ? b.Min.Y : b.Max.Y;
        float fz = fabsf(c.Z - b.Min.Z) > fabsf(c.Z - b.Max.Z) ? b.Min.Z : b.Max.Z;
        float dx = c.X - fx, dy = c.Y - fy, dz = c.Z - fz;
        if (dz * dz + dy * dy + dx * dx > v.Radius * v.Radius) inside = false;
    }
    for (int i = 0; i < v.Count; ++i) {
        const Plane& p = v.Planes[i];
        float x = p.N.X > 0 ? b.Min.X : b.Max.X, y = p.N.Y > 0 ? b.Min.Y : b.Max.Y, z = p.N.Z > 0 ? b.Min.Z : b.Max.Z;
        if (x * p.N.X + y * p.N.Y + z * p.N.Z > p.D) { inside = false; return false; }
        if (inside) {
            float fx = p.N.X > 0 ? b.Max.X : b.Min.X, fy = p.N.Y > 0 ? b.Max.Y : b.Min.Y, fz = p.N.Z > 0 ? b.Max.Z : b.Min.Z;
            if (fx * p.N.X + fy * p.N.Y + fz * p.N.Z > p.D) inside = false;
        }
    }
    return true;
}
int main()
{
    Box b = { { 0, 0, 0 }, { 10, 10, 10 } };
    Volume v = {};
    v.ClipToSphere = true; v.Centre.X = 5; v.Centre.Y = 5; v.Centre.Z = 5; v.Radius = 100;
    v.Count = 1; v.Planes[0].N.X = 1; v.Planes[0].D = 20;
    bool in = false; int ok = 1;
    ok &= Clip(v, b, in) && in;                                       // inside sphere and behind the plane
    v.Radius = 8; ok &= Clip(v, b, in) && !in;                        // far corner (distance ~8.66) outside the sphere
    v.Radius = 100; v.Planes[0].D = 5; ok &= Clip(v, b, in) && !in;   // straddles the plane
    v.Planes[0].D = -1; ok &= !Clip(v, b, in) && !in;                 // fully in front of the plane
    v.Centre.X = 200; ok &= !Clip(v, b, in) && !in;                   // outside the sphere
    printf(ok ? "00b4ced0_TEST PASS\n" : "00b4ced0_TEST FAIL\n");
    return ok ? 0 : 1;
}