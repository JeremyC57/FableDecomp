// Behaviour test for 0x00BBBF70 CEnginePrimitiveManagerParticleGroup::RenderPrimitive (model copy).
#include <cstdio>
#include <cmath>

struct V3 { float X, Y, Z; };
struct Group { V3 WorldPos; bool ScreenDisplacement; long WaterStage; bool ZBufferWritable; };
static int g_lists[8]; static float g_lastDistance; static bool g_displacementPass = true;
static void Add(long list, float d) { ++g_lists[list]; g_lastDistance = d; }
static long Render(bool enable, const Group& g, const V3& cam)
{
    if (!enable) return 0;
    V3 d = { g.WorldPos.X - cam.X, g.WorldPos.Y - cam.Y, g.WorldPos.Z - cam.Z };
    float distance = (float)std::sqrt(d.X * d.X + d.Y * d.Y + d.Z * d.Z);
    if (g.ScreenDisplacement) {
        if (g_displacementPass) { Add(5, distance); Add(6, distance); }
    } else
        Add(g.WaterStage != 0 && !g.ZBufferWritable ? 1 : 0, distance);
    return 2;
}

int main()
{
    V3 cam = { 0, 0, 0 };
    Group g = { { 3, 4, 0 }, false, 0, false };
    if (Render(false, g, cam) != 0 || Render(true, g, cam) != 2 || g_lists[0] != 1 || g_lastDistance != 5.0f)
    { std::printf("00bbbf70_TEST FAIL\n"); return 1; }
    g.WaterStage = 2; Render(true, g, cam);
    g.ZBufferWritable = true; Render(true, g, cam);
    g.ScreenDisplacement = true; Render(true, g, cam);
    g_displacementPass = false; Render(true, g, cam);
    if (g_lists[1] != 1 || g_lists[0] != 2 || g_lists[5] != 1 || g_lists[6] != 1) { std::printf("00bbbf70_TEST FAIL\n"); return 1; }
    std::printf("00bbbf70_TEST PASS\n");
    return 0;
}