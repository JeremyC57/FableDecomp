// CMainGameComponent::SetExclusiveMode(bool exclusive) -- notify the display object (+0x28), switch the system manager's exclusive (full-screen) mode, then re-run the post-resolution-change setup with the back surface's width, height and colour depth
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004163B7, length 104 bytes.
// Callees: CDisplayEngine::PreChangeResolution @ 0x00434A10 on PDisplayEngine (+0x28; FableWin.pdb ?PreChangeResolution@CDisplayEngine@@QAEXXZ,
//          engine/CDisplayEngine.h; NF166 q4523: was the stand-in CDisplayHolder::OnExclusiveModeChange),
//          GFGetSystemManager @ 0x009A4EC0, CSystemManager::SetExclusiveMode(bool) @ 0x009A59A0,
//          CSurface::GetDimensions @ 0x009F2F90 (C2DExtentsI by hidden pointer), CSurface::GetPixelFormat
//          @ 0x009F2F60 (CPixelFormat by hidden pointer), CPixelFormat::GetColourDepth @ 0x009E3820,
//          CSurface::GetHeight @ 0x00415BF0, CMainGameComponent::PostChangeResolution(ulong, ulong, ulong) @ 0x00416365.
// Globals: the display device at 0x013B8390, its back surface embedded at +0x2c.
// Shape: size TU with a frame pointer (`optimize("s", on)` + `("y", off)`); the width is a local read before the
// call's other arguments (which evaluate right to left: depth, then height); the surface pointer local serves the
// width and the height while the pixel format goes through the global again; the CPixelFormat temporary lands in
// the dead argument slot.
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_CDISPLAYENGINE
#include "engine/CDisplayEngine.h"
class CSystemManager { public:
    bool SetExclusiveMode(bool exclusive);         // 0x009A59A0
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
class C2DExtentsI { public: long Width; long Height; };
class CPixelFormat { public:
    unsigned long m_format;
    unsigned long GetColourDepth() const;          // 0x009E3820
};
class CSurface { public:
    C2DExtentsI   GetDimensions() const;           // 0x009F2F90
    CPixelFormat  GetPixelFormat() const;          // 0x009F2F60
    unsigned long GetHeight() const;               // 0x00415BF0
};
struct CDisplayDevice {
    unsigned char _pad00[0x2c];
    CSurface      m_backSurface;                   // +0x2c
};
class CDisplayManager;
extern CDisplayManager* GPDisplayManager;           // 0x013B8390 (naming wave 13)
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* m_pDisplay;                    // +0x28
    void PostChangeResolution(unsigned long width, unsigned long height, unsigned long depth);   // 0x00416365
    void SetExclusiveMode(bool exclusive);
};

void CMainGameComponent::SetExclusiveMode(bool exclusive)
{
    m_pDisplay->PreChangeResolution();
    GFGetSystemManager()->SetExclusiveMode(exclusive);
    CSurface* surface = &((CDisplayDevice*)GPDisplayManager)->m_backSurface;
    unsigned long width = surface->GetDimensions().Width;
    PostChangeResolution(width, surface->GetHeight(), ((CDisplayDevice*)GPDisplayManager)->m_backSurface.GetPixelFormat().GetColourDepth());
}