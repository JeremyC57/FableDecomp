// NGraphicAppearance::CTCGraphicAppearance::GetFullScale() const -- the main graphic's render size times the appearance scale.
// Leaf: ?GetFullScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BBFF0, 7 bytes: fld dword ptr [ecx+0x4c] ; fmul dword ptr [ecx+0x34] ; ret
// Name: Ego_r.pdb 0x004BF740 + FableWin.pdb 0x01D0A3C0 ?GetFullScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ, out of line;
//   the inline GetScale reads Scale (+0x34) alone. Its retail callers 0x00724630 / 0x00727D00 / 0x00747330 / 0x0075A530 /
//   0x0077C4D0 bind this symbol (NF33 2026-11-16; formerly the made-up free function CMap_DrawGetSeaLevel).
// Layout: engine/NGraphicAppearance_CTCGraphicAppearance.h retail branch (_MOVEMENT): Scale +0x34, MainGraphic +0x44
//   (CEngineGraphic RenderSizeX +0x08 = +0x4C).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
namespace NGraphicAppearance {
float CTCGraphicAppearance::GetFullScale() const
{
    return MainGraphic.RenderSizeX * Scale;
}
}
