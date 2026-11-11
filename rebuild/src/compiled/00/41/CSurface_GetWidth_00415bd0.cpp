// CSurface::GetWidth() const -- the surface's width (GetDimensions().X), the out-of-line copy of the header inline
// Leaf: ?GetWidth@CSurface@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415BD0, thiscall, plain `ret`: `sub esp,8 ; lea eax,[esp] ; push eax ;
//   call 0x009F2F90 ; mov eax,[eax] ; add esp,8`.
// Name (agentNF20, 2026-11-11): Ego_r.pdb + FableWin.pdb ?GetWidth@CSurface@@QBEKXZ; it calls ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ
//   0x009F2F90 (was the stand-in CSurface_Methods::GetRect / Get). engine/CSurface.h: FABLE_RETAIL_CSURFACE_BITMAP +
//   opt-in FABLE_RETAIL_CSURFACE_SIZE_OUTOFLINE (declaration only).
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_SIZE_OUTOFLINE
#include "engine/C2DExtentsI.h"
#include "engine/CSurface.h"

unsigned long CSurface::GetWidth() const
{
    return GetDimensions().X;
}
