// NGraphicAppearance::CTCGraphicAppearance::BulletTimeFrameUpdate() -- the bullet-time frame update is the ordinary frame update (virtual, slot 9)
// Leaf: ?BulletTimeFrameUpdate@CTCGraphicAppearance@NGraphicAppearance@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C5D70, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 10 of NGraphicAppearance::CTCGraphicAppearance's vtable 0x0123986C (the slot holds this body in 1 vtable(s), owner NGraphicAppearance::CTCGraphicAppearance); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts BulletTimeFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?BulletTimeFrameUpdate@CTCGraphicAppearance@NGraphicAppearance@@UAEXXZ; no `ret` to check (NORET).
// Was: NGraphicAppearance::CTCGraphicAppearance::vfunc_10 (rebuild/src/compiled/00/4c/NGraphicAppearanceCTCGraphicAppearance_vfunc10_004c5d70.cpp).
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_VSLOTS3
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
namespace NGraphicAppearance {
void CTCGraphicAppearance::BulletTimeFrameUpdate()
{
    FrameUpdate();
}
}
