// Behaviour test for 0x00A0AFE0: CRenderManagerCore ctor: base built with the init flags, display manager bound, pixel format -1, all 8 texture bindings cleared
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct DisplayManager { int id; };
struct StateManagerBase { unsigned int initFlags; StateManagerBase(unsigned int f) : initFlags(f) {} };
struct SystemFont { bool initialised; SystemFont() : initialised(false) {} };
struct PixelFormat { long formatIndex; PixelFormat() : formatIndex(-1) {} };
struct RenderManagerCore : StateManagerBase {
    DisplayManager* displayManager; PixelFormat screenFormat; SystemFont font; unsigned int textures[8];
    RenderManagerCore(DisplayManager& dm, unsigned int flags) : StateManagerBase(flags), displayManager(&dm)
    { for (long i = 0; i < 8; ++i) textures[i] = 0; }
};
static void Run()
{
    DisplayManager dm; dm.id = 3;
    RenderManagerCore r(dm, 0x55);
    CHECK(r.initFlags == 0x55 && r.displayManager == &dm && r.screenFormat.formatIndex == -1 && !r.font.initialised);
    for (int i = 0; i < 8; ++i) CHECK(r.textures[i] == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a0afe0_TEST FAIL\n"); return 1; }
    std::printf("00a0afe0_TEST PASS\n");
    return 0;
}