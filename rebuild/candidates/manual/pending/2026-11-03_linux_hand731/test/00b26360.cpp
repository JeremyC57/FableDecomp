#include <cstdio>
#include <cstring>
// Model of CEngine::Initialise: uninitialise first; with a definition manager look up ENGINE (index 1) and the four
// ENGINE_VIDEO_OPTIONS defs; create the 18 always-present components in retail order (only the resource manager is
// added with flag 1), the 9 world components and the ENVIRONMENT def only when InitFlags bit 0 is set (the local detail
// generator gets InitFlags bit 2), build the 34 render layers, then the initial state (wireframe off, textures on,
// anisotropy 2, 15 world fps, scene filter 3, off-world tick processing enabled at 10 / 3 / 10) and return true.
static const char* g_defs[8]; static int g_ndefs = 0;
static int g_comps = 0, g_flagged = 0, g_layers = 0, g_uninit = 0; static bool g_detailFlag = false;
static void GetDef(const char* name) { if (g_ndefs < 8) g_defs[g_ndefs] = name; ++g_ndefs; }
static void Add(unsigned char flag) { ++g_comps; if (flag) ++g_flagged; }
struct Engine {
    bool EnableWireFrame, EnableTextures, IsInitialised; long Aniso, WorldFPS; unsigned long SceneFilter;
    bool OffWorldProcessing; long MaxBehind, MaxBehindBeforeCatchup, MinFreq;
    bool Initialise(bool haveDefs, long flags)
    {
        ++g_uninit;
        if (haveDefs) { GetDef("ENGINE"); GetDef("ENGINE_VIDEO_OPTIONS_LOW"); GetDef("ENGINE_VIDEO_OPTIONS_NORMAL");
                        GetDef("ENGINE_VIDEO_OPTIONS_HIGH"); GetDef("ENGINE_VIDEO_OPTIONS_HIGHEST"); }
        Add(0); Add(1); for (int i = 0; i < 16; ++i) Add(0);
        if (flags & 1) { GetDef("ENVIRONMENT"); for (int i = 0; i < 9; ++i) Add(0); g_detailFlag = (flags & 4) != 0; }
        g_layers = 34;
        EnableWireFrame = false; EnableTextures = true; IsInitialised = true; Aniso = 2; WorldFPS = 15; SceneFilter = 3;
        OffWorldProcessing = true; MaxBehind = 10; MaxBehindBeforeCatchup = 3; MinFreq = 10;
        return true;
    }
};
int main()
{
    int bad = 0; Engine e;
    if (!e.Initialise(true, 0) || g_uninit != 1 || g_ndefs != 5 || g_comps != 18 || g_flagged != 1 || g_layers != 34) bad = 1;
    if (std::strcmp(g_defs[0], "ENGINE") != 0 || std::strcmp(g_defs[4], "ENGINE_VIDEO_OPTIONS_HIGHEST") != 0) bad = 1;
    g_ndefs = 0; g_comps = 0; g_flagged = 0;
    if (!e.Initialise(true, 5) || g_ndefs != 6 || std::strcmp(g_defs[5], "ENVIRONMENT") != 0 || g_comps != 27 || !g_detailFlag) bad = 1;
    g_ndefs = 0; g_comps = 0;
    if (!e.Initialise(false, 1) || g_ndefs != 1 || g_comps != 27 || g_detailFlag) bad = 1;
    if (e.EnableWireFrame || !e.EnableTextures || !e.IsInitialised || e.Aniso != 2 || e.WorldFPS != 15 || e.SceneFilter != 3) bad = 1;
    if (!e.OffWorldProcessing || e.MaxBehind != 10 || e.MaxBehindBeforeCatchup != 3 || e.MinFreq != 10) bad = 1;
    if (bad) { std::printf("00b26360_TEST FAIL\n"); return 1; }
    std::printf("00b26360_TEST PASS\n");
    return 0;
}
