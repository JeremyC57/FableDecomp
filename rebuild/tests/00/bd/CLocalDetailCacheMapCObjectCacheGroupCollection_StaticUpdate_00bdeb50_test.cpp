#include <cstdio>
#include <math.h>
struct V { float X, Y, Z; };
static V g_cam; static double g_now = 0.0; static float g_fade = 0.0f;
struct Coll {
    V Centre; float Radius; float MaxFadeDistance; unsigned int PrimitiveTypeMask; int loads, unloads;
    bool LoadContents() { ++loads; return true; }
    void UnloadContents() { ++unloads; }
    bool StaticUpdate(double deadline)
    {
        float dx = Centre.X - g_cam.X, dy = Centre.Y - g_cam.Y, dz = Centre.Z - g_cam.Z;
        float distance = (float)sqrt(dx * dx + dy * dy + dz * dz) - Radius;
        if (g_fade > distance) { if (g_now < deadline) return LoadContents(); return false; }
        UnloadContents();
        return false;
    }
};
int main()
{
    Coll c = { { 3, 4, 0 }, 1, 100, 1, 0, 0 };
    g_cam.X = g_cam.Y = g_cam.Z = 0; g_fade = 10.0f; g_now = 1.0;
    bool fail = false;
    if (!c.StaticUpdate(2.0) || c.loads != 1) fail = true;          // distance 4 < 10
    if (c.StaticUpdate(0.5) || c.loads != 1 || c.unloads) fail = true; // out of time
    g_fade = 3.0f; if (c.StaticUpdate(2.0) || c.unloads != 1) fail = true;
    if (fail) { std::printf("00bdeb50_TEST FAIL\n"); return 1; }
    std::printf("00bdeb50_TEST PASS\n");
    return 0;
}