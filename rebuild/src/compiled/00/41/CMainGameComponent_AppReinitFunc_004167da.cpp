// CMainGameComponent::AppReinitFunc(void* component) -- system-manager re-init callback: tell the display engine a resolution change is coming, then pass the back buffer's width, height and colour depth to PostChangeResolution
// Leaf: ?AppReinitFunc@CMainGameComponent@@KIXPAX@Z
// Retail: Fable.exe @ 0x004167DA, 88 bytes, plain `ret` (static __fastcall, the component in ecx); its address is
// registered at 0x0041899B (CSystemManager::SetAppReinitFunc takes `void (__fastcall*)(void*)`); Ego_r.pdb
// ?AppReinitFunc@CMainGameComponent@@KIXPAX@Z.
// Body: PDisplayEngine (+0x28) ->PreChangeResolution() (0x00434A10); then on the display manager's (0x013B8390)
// BackBufferSurface (+0x2c): PostChangeResolution(surface.GetWidth(), surface.GetHeight() (0x00415BF0),
// BackBufferSurface.GetPixelFormat() (0x009F2F60, CPixelFormat by hidden pointer).GetColourDepth() (0x009E3820))
// (0x00416365), GetWidth being the inline GetDimensions().X (0x009F2F90, C2DExtentsI by hidden pointer).
// Layout: Ego_r.pdb CMainGameComponent PDisplayEngine +0x28 (as 0x0041799B); CDisplayManager BackBufferSurface +0x2c
// (CSurface, engine/CSurface.h); CPixelFormat (engine/CPixelFormat.h) and C2DExtentsI {X, Y} returned through memory.
// Shape: frame-pointer size TU; GetWidth is an inline accessor (forced: the size TU would keep it out of line) -- the
// inlined argument is evaluated first and its C2DExtentsI temporary keeps its own slot (`sub esp,0Ch`); a plain
// `GetDimensions().X` argument or local shares the slot with the CPixelFormat temporary. The surface address stays in edi
// for the width and height while the depth re-reads the global.
#pragma optimize("s", on)
#pragma optimize("y", off)
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