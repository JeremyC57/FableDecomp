// behaviour test for 0x004BD620 CTCGraphicAppearance::SetPrimitiveGraphics
#include <cstdio>
struct Scene { int oneOff; int persistent; unsigned int lastFlags;
    void SetPrimitive() { ++oneOff; } void SetPersistentPrimitive(unsigned int f) { ++persistent; lastFlags = f; } };
static Scene g_scene = { 0, 0, 0 };
struct App {
    unsigned short Active; bool DisableOcclusion; bool afterWater; signed char ByPass;
    void Set(bool havePrim, unsigned short flags)
    {
        if (havePrim)
        {
            unsigned short f = Active | flags;
            if (ByPass > 0) f |= 0x800;
            if (DisableOcclusion) f |= 0x4000;
            if (afterWater) f |= 8;
            g_scene.SetPersistentPrimitive(f);
        }
        else g_scene.SetPrimitive();
    }
};
int main()
{
    App a = { 0x1, true, true, 1 };
    a.Set(true, 0x10);
    bool ok = g_scene.persistent == 1 && g_scene.lastFlags == (0x1u | 0x10u | 0x800u | 0x4000u | 8u);
    a.ByPass = -1; a.DisableOcclusion = false; a.afterWater = false; a.Set(true, 0);
    ok = ok && g_scene.lastFlags == 1;
    a.Set(false, 0); ok = ok && g_scene.oneOff == 1;
    if (!ok) { std::printf("004bd620_TEST FAIL\n"); return 1; }
    std::printf("004bd620_TEST PASS\n");
    return 0;
}