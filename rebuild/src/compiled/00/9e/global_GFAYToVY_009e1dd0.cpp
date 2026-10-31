// GFAYToVY(float) -- convert an actual render-target Y to virtual coordinates: VIRTUAL_SCREEN_HEIGHT * y / height
// Leaf: ?GFAYToVY@@YIMM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E1DD0, 49 bytes (+ int3 padding), __fastcall float(float), `ret 4`.
// NOT A CHUNK 649 ROW: found while matching GFACoordToVCoord 0x009E21D0 (which inlines it); graded RELOCATION_MATCH
// against the 0x009E1DD0 oracle with q.py (tag a649_1dd0). Parked elsewhere as 2026-10-31_codex_parked50/differ/009e1dd0.cpp.
// Callees: GFGetSystemManager @ 0x009A4EC0, CDisplayManager::GetRenderTargetDimensions @ 0x009BEDC0 (const C2DExtentsI by
// hidden pointer). Global: VIRTUAL_SCREEN_HEIGHT @ 0x013961F0 (Ego_r data symbol ?VIRTUAL_SCREEN_HEIGHT@@3NA).
// Shape: `(float)((float)VIRTUAL_SCREEN_HEIGHT * y) / dims.Y` -- the explicitly truncated product is evaluated before the
// dimensions call and spilled into y's argument slot between `push ecx` and `mov ecx,eax`, as retail; `y = (float)H * y;`
// stores it after `mov ecx,eax`.
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
extern double VIRTUAL_SCREEN_HEIGHT;                            // 0x013961F0

float __fastcall GFAYToVY(float y)                       // 0x009E1DD0 out of line
{
    CSystemManager* system = GFGetSystemManager();
    return (float)((float)VIRTUAL_SCREEN_HEIGHT * y) / system->GetDisplayManager()->GetRenderTargetDimensions().Y;
}
