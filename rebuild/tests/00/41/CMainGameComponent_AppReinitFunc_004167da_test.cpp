#include <cstdio>
#include "engine/CSurface.h"
#include "engine/CPixelFormat.h"
class C2DExtentsI {
public:
    long X;
    long Y;
    C2DExtentsI() {}
};
struct CPixelFormat_Methods : CPixelFormat {
    CPixelFormat_Methods() {}
    unsigned long GetColourDepth() const;                // 0x009E3820
};
struct CSurface_Methods : CSurface {
    const C2DExtentsI GetDimensions() const;             // 0x009F2F90
    const CPixelFormat_Methods GetPixelFormat() const;   // 0x009F2F60
    __forceinline unsigned long GetWidth() const { return GetDimensions().X; }
    unsigned long GetHeight() const;                     // 0x00415BF0 (out of line: GetDimensions().Y)
};
class CDisplayManager {
public:
    unsigned char    _pad00[0x2c];
    CSurface_Methods BackBufferSurface;                  // +0x2c
};
extern CDisplayManager* g_pDisplayManager;               // 0x013B8390
class CDisplayEngine {
public:
    void PreChangeResolution();                          // 0x00434A10
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                      // +0x28 scoped_ptr<CDisplayEngine>
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x00416365
protected:
    static void __fastcall AppReinitFunc(void* component);
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
void __fastcall CMainGameComponent::AppReinitFunc(void* component)
{
    CMainGameComponent* game = (CMainGameComponent*)component;
    game->PDisplayEngine->PreChangeResolution();
    const CSurface_Methods& surface = g_pDisplayManager->BackBufferSurface;
    game->PostChangeResolution(surface.GetWidth(), surface.GetHeight(), g_pDisplayManager->BackBufferSurface.GetPixelFormat().GetColourDepth());
}

static int g_pre, g_order, g_dim_order, g_fmt_order, g_h_order;
static unsigned long g_w, g_h, g_d;
static CDisplayManager g_dm;
CDisplayManager* g_pDisplayManager = &g_dm;
void CDisplayEngine::PreChangeResolution() { ++g_pre; ++g_order; }
const C2DExtentsI CSurface_Methods::GetDimensions() const { g_dim_order = ++g_order; C2DExtentsI e; e.X = 800; e.Y = 600; return e; }
const CPixelFormat_Methods CSurface_Methods::GetPixelFormat() const { g_fmt_order = ++g_order; CPixelFormat_Methods f; f.FormatIndex = 7; return f; }
unsigned long CSurface_Methods::GetHeight() const { g_h_order = ++g_order; return 600; }
unsigned long CPixelFormat_Methods::GetColourDepth() const { return FormatIndex == 7 ? 32 : 16; }
void CMainGameComponent::PostChangeResolution(unsigned long w, unsigned long h, unsigned long d) { g_w = w; g_h = h; g_d = d; }
struct CMainGameComponent_Test { static void Run(void* p) { CMainGameComponent::AppReinitFunc(p); } };
int main()
{
    CMainGameComponent c; CDisplayEngine e; c.PDisplayEngine = &e;
    CMainGameComponent_Test::Run(&c);
    bool ok = g_pre == 1 && g_w == 800 && g_h == 600 && g_d == 32 && g_dim_order > 1 && g_fmt_order > 1 && g_h_order > 1;
    std::printf(ok ? "004167da_TEST PASS\n" : "004167da_TEST FAIL\n");
    return ok ? 0 : 1;
}