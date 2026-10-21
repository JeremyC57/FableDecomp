// behaviour test for 00b930a0 CEngineScene::PreloadPrimitives: every scene map grid is preloaded from cell (0,0); a moving
// primitive is preloaded only when its bounding sphere is valid, overlaps the volume's sphere (when enabled) and lies
// on the inner side of every clip plane; the result ORs every preload
#include <cstdio>
struct Sphere { float x, y, z, r; };
struct Plane { float nx, ny, nz, d; };
struct Volume { Plane planes[8]; int count; Sphere bound; bool clipToSphere; };
static bool Inside(const Volume& v, const Sphere& s)
{
    if (v.clipToSphere) {
        float dx = s.x - v.bound.x, dy = s.y - v.bound.y, dz = s.z - v.bound.z, r = s.r + v.bound.r;
        if (dx * dx + dy * dy + dz * dz > r * r) return false;
    }
    for (int i = 0; i < v.count; ++i)
        if (s.x * v.planes[i].nx + s.y * v.planes[i].ny + s.z * v.planes[i].nz - s.r > v.planes[i].d) return false;
    return true;
}
struct Prim { bool valid; Sphere s; bool loads; int preloads; };
static int g_gridCalls = 0;
static bool Preload(const Volume& v, Prim* prims, int n, int grids, bool gridResult)
{
    bool result = false;
    for (int g = 0; g < grids; ++g) { ++g_gridCalls; result |= gridResult; }
    for (int i = 0; i < n; ++i) {
        Sphere s = { 0, 0, 0, 0 };
        if (prims[i].valid) s = prims[i].s; else continue;
        if (!Inside(v, s)) continue;
        ++prims[i].preloads;
        result |= prims[i].loads;
    }
    return result;
}
int main()
{
    Volume v;
    v.count = 1; v.planes[0].nx = 1; v.planes[0].ny = 0; v.planes[0].nz = 0; v.planes[0].d = 10;
    v.bound.x = 0; v.bound.y = 0; v.bound.z = 0; v.bound.r = 5; v.clipToSphere = true;
    Prim p[3] = { { true, { 1, 1, 1, 1 }, false, 0 }, { true, { 20, 0, 0, 1 }, true, 0 }, { false, { 0, 0, 0, 0 }, true, 0 } };
    bool r = Preload(v, p, 3, 2, false);
    if (r || g_gridCalls != 2 || p[0].preloads != 1 || p[1].preloads || p[2].preloads) { std::printf("00b930a0_TEST FAIL\n"); return 1; }
    v.clipToSphere = false; v.planes[0].d = 30; p[1].preloads = 0;
    r = Preload(v, p, 3, 0, false);
    if (!r || p[1].preloads != 1) { std::printf("00b930a0_TEST FAIL\n"); return 1; }
    std::printf("00b930a0_TEST PASS\n");
    return 0;
}
