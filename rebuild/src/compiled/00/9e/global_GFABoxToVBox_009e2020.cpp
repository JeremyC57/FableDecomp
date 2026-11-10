// GFABoxToVBox(const C2DBoxF&, C2DBoxF&) -- convert an actual (render-target pixel) float box to virtual coordinates: each corner's X scaled by VIRTUAL_SCREEN_WIDTH / width, each Y by VIRTUAL_SCREEN_HEIGHT / height
// Leaf: ?GFABoxToVBox@@YIXABVC2DBoxF@@AAV1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2020, length 199 bytes (+ int3 padding), __fastcall (a in ecx, v in edx), plain `ret`.
// Identity: Ego_r.pdb ?GFABoxToVBox@@YIXABVC2DBoxF@@AAV1@@Z (the float overload; the C2DBoxI one is the landed 0x009E20F0).
// Callees: GFGetSystemManager 0x009A4EC0, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (x4); globals VIRTUAL_SCREEN_WIDTH
//   0x013961E8 / VIRTUAL_SCREEN_HEIGHT 0x013961F0.
// Shape: the four corners through the same-TU inline GFAXToVX / GFAYToVY (out of line at 0x009E1DA0 / 0x009E1DD0) exactly as the
//   landed sibling GFACoordToVCoord 0x009E21D0 spells them: the Y halves' explicitly truncated product
//   `(float)((float)VIRTUAL_SCREEN_HEIGHT * y)` is spilled between `push ecx` and `mov ecx,eax` (0x009E206F / 0x009E20CC) as retail.
// Layout: shared engine/C2DBoxF.h, C2DExtentsI.h, CDisplayManager.h (FABLE_RETAIL_CDISPLAYMANAGER: GetRenderTargetDimensions) and
//   CSystemManager.h (FABLE_RETAIL_CSYSTEMMANAGER: PDisplayManager +0x60 and the inline GetDisplayManager()).
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxF.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"

CSystemManager* __fastcall GFGetSystemManager();                // 0x009A4EC0
extern double VIRTUAL_SCREEN_WIDTH;                             // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                            // 0x013961F0

inline float __fastcall GFAXToVX(float x)                       // 0x009E1DA0 out of line
{
    CSystemManager* system = GFGetSystemManager();
    return ((float)VIRTUAL_SCREEN_WIDTH * x) / system->GetDisplayManager()->GetRenderTargetDimensions().X;
}

inline float __fastcall GFAYToVY(float y)                       // 0x009E1DD0 out of line
{
    CSystemManager* system = GFGetSystemManager();
    return (float)((float)VIRTUAL_SCREEN_HEIGHT * y) / system->GetDisplayManager()->GetRenderTargetDimensions().Y;
}

void __fastcall GFABoxToVBox(const C2DBoxF& a, C2DBoxF& v)
{
    v.TLX = GFAXToVX(a.TLX);
    v.TLY = GFAYToVY(a.TLY);
    v.BRX = GFAXToVX(a.BRX);
    v.BRY = GFAYToVY(a.BRY);
}