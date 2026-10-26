// FIDELITY NOTE: the bytes match retail, but the source may not be the original. `volatile` on
// g_FableVirtualWidth is a compiler lever with no evidence behind it: the global is a plain .data double
// (65536.0) written only by CDisplayManager::InitVirtualPixels on the main thread. Without it VC7.1 emits
// `fld x; fmul qword g` where retail has `fld qword g; fmul dword x`; 36 volatile-free rewrites failed
// (2026-10-23 audit). Kept landed at the owner's request; check the PDB cv-qualifier on the Windows box.
#include "rebuild_abi.h"

class C2DExtentsI
{ public:
    fable_i32 width;
    fable_i32 height;
};

struct CDisplayManagerCoordinateView
{
    C2DExtentsI GetRenderTargetDimensions() const;
};

struct CSystemManagerCoordinateView
{
    fable_u8 unknown000[0x60];
    CDisplayManagerCoordinateView* displayManager60;
};

extern volatile double g_FableVirtualWidth_013961E8;

CSystemManagerCoordinateView* FABLE_FASTCALL GFGetSystemManager();

float FABLE_FASTCALL GFAXToVX(float x)
{
    CDisplayManagerCoordinateView* displayManager =
        GFGetSystemManager()->displayManager60;
    return static_cast<float>(
        (g_FableVirtualWidth_013961E8 * x) /
        displayManager->GetRenderTargetDimensions().width);
}
