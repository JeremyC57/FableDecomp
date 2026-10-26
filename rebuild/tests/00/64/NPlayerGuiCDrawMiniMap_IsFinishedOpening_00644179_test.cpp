#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

static bool g_faded, g_showing, g_open; static int g_openCalls = 0;
class CDisplayEngine { public: bool IsScreenFullyFadedOut() { return g_faded; } };
class CMainGameComponent { public: unsigned char p[0x28]; CDisplayEngine* m_pDisplayEngine; };
class CWorld { public: bool IsDisplayingMiniMapOnRegionLoad() { return g_showing; } };
class CFadeInFadeOutBase { public: unsigned char Active; unsigned char r[0x1b]; bool IsOpen() { g_openCalls++; return g_open; } };
struct CDrawMiniMap { unsigned char p0[0xc]; CWorld* World; CMainGameComponent* Component; unsigned char p1[0xe8 - 0x14]; unsigned char Enabled, Active, OldActive, RegionChange; unsigned char Alpha; unsigned char p2[0x100 - 0xed]; CFadeInFadeOutBase FadeInOut;
    bool IsFinishedOpening() { if (RegionChange && Active && Component->m_pDisplayEngine->IsScreenFullyFadedOut() && World->IsDisplayingMiniMapOnRegionLoad() && FadeInOut.Active) return FadeInOut.IsOpen(); return true; } };
static CDisplayEngine g_eng; static CMainGameComponent g_comp; static CWorld g_world;
static void setup(CDrawMiniMap& m, bool rc, bool act, bool faded, bool showing, bool fadeActive) { g_comp.m_pDisplayEngine = &g_eng; m.World = &g_world; m.Component = &g_comp; m.RegionChange = rc; m.Active = act; g_faded = faded; g_showing = showing; m.FadeInOut.Active = fadeActive; }

int main() { CDrawMiniMap m; setup(m, true, true, true, true, true); g_open = false; if (m.IsFinishedOpening() || g_openCalls != 1) { std::printf("00644179_TEST FAIL\n"); return 1; }
    g_open = true; if (!m.IsFinishedOpening()) { std::printf("00644179_TEST FAIL\n"); return 1; }
    setup(m, false, true, true, true, true); g_open = false; if (!m.IsFinishedOpening() || g_openCalls != 2) { std::printf("00644179_TEST FAIL\n"); return 1; }
    setup(m, true, true, true, true, false); if (!m.IsFinishedOpening() || g_openCalls != 2) { std::printf("00644179_TEST FAIL\n"); return 1; }
    std::printf("00644179_TEST PASS\n"); return 0; }