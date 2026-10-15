#include <cstdio>
// Behaviour test for 0064e83f.

struct CPlayerGui { int id; };
struct CRetailHandleVector { unsigned int reserved; CRetailHandleVector() : reserved(0) { } void Resize(long n) { reserved = (unsigned int)n; } };
struct CRetailEmitterVector { void* m_pFirst; void* m_pLast; void* m_pEnd; unsigned int reserved;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), reserved(0) { } void Resize(long n) { reserved = (unsigned int)n; } };
struct CFadeInFadeOutBase { int built; CFadeInFadeOutBase() : built(1) { } };
struct CDrawBase {
    CPlayerGui* PlayerGui; void* DrawList; CRetailHandleVector PrimitiveHandles;
    CDrawBase(CPlayerGui* gui, void* list) : PlayerGui(gui), DrawList(list) { }
    virtual ~CDrawBase() { }
};

// NPlayerGui::CDrawMiniMap::CDrawMiniMap: display built, flags enabled/active, alpha 255, fades built, display initialised with
// the GUI def's world resolution, nine primitive handles reserved.
struct CPlayerGuiDef { float MiniMapWorldResolution; };
static CPlayerGuiDef g_def;
static CPlayerGuiDef* PeekGuiDef() { return &g_def; }
struct CMiniMapDisplay { float resolution; CMiniMapDisplay() : resolution(-1.0f) { } void Initialise(float r) { resolution = r; } };
struct CDrawMiniMap : CDrawBase {
    CMiniMapDisplay MiniMap; bool Enabled, Active, OldActive, RegionChange; unsigned char Alpha;
    CFadeInFadeOutBase FadeInOut, TextFadeInOut, ScreenPosFadeInOut; void* RegionExits;
    CDrawMiniMap(CPlayerGui* gui, void* drawList)
        : CDrawBase(gui, drawList), MiniMap(), Enabled(true), Active(true), OldActive(true), RegionChange(false), Alpha(0xff),
          FadeInOut(), TextFadeInOut(), ScreenPosFadeInOut(), RegionExits(0)
    { MiniMap.Initialise(PeekGuiDef()->MiniMapWorldResolution); PrimitiveHandles.Resize(9); }
};
int main()
{
    g_def.MiniMapWorldResolution = 4.0f;
    CPlayerGui gui;
    CDrawMiniMap m(&gui, 0);
    int ok = m.Enabled && m.Active && m.OldActive && !m.RegionChange && m.Alpha == 0xff && m.MiniMap.resolution == 4.0f
          && m.PrimitiveHandles.reserved == 9 && m.ScreenPosFadeInOut.built;
    if (!ok) { std::printf("0064e83f_TEST FAIL\n"); return 1; }
    std::printf("0064e83f_TEST PASS\n");
    return 0;
}
