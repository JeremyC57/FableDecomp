// Re-authored 2026-10-31 (chunk 649 lesson): the explicit float truncation of the product replaces the former `volatile` lever.
// GFAXToVX(float) -- convert an actual render-target X to virtual coordinates: VIRTUAL_SCREEN_WIDTH * x / width
// Leaf: ?GFAXToVX@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1DA0, 40 bytes (+ int3 padding), __fastcall float(float), `ret 4`.
// Callees: GFGetSystemManager @ 0x009A4EC0, CDisplayManager::GetRenderTargetDimensions @ 0x009BEDC0 (const C2DExtentsI by
// hidden pointer). Global: VIRTUAL_SCREEN_WIDTH @ 0x013961E8 (Ego_r data symbol ?VIRTUAL_SCREEN_WIDTH@@3NA).
// Shape: the product is formed after the dimensions call (no spill): `fld qword W; fmul dword x; fidiv dword [eax]`.
#include "engine/CCountedPointer.h"

class C2DExtentsI {
public:
    long X;                                                     // +0x0
    long Y;                                                     // +0x4
    C2DExtentsI();
    C2DExtentsI(long x, long y);
    C2DExtentsI(const C2DExtentsI& other);
};

class CDisplayManager {
public:
    const C2DExtentsI GetRenderTargetDimensions();              // 0x009BEDC0
};

class CSystemManager {
public:
    unsigned char _pad_0x00[0x60];                              // CInitBaseClass base .. LibDebugManager / PInputManager
    CCountedPointer<CDisplayManager> PDisplayManager;          // +0x60
    CDisplayManager* GetDisplayManager() { return PDisplayManager.Get(); }   // Ego_r ?GetDisplayManager@CSystemManager@@QAEPAVCDisplayManager@@XZ
};

CSystemManager* __fastcall GFGetSystemManager();                // 0x009A4EC0
extern double VIRTUAL_SCREEN_WIDTH;                             // 0x013961E8

float __fastcall GFAXToVX(float x)                       // 0x009E1DA0 out of line
{
    CSystemManager* system = GFGetSystemManager();
    return (float)((float)VIRTUAL_SCREEN_WIDTH * x) / system->GetDisplayManager()->GetRenderTargetDimensions().X;
}
