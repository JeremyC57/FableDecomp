// NHeroInformationScreens::CBase::IsInventoryScreen() const -- the base information screen is an inventory screen
// Leaf: ?IsInventoryScreen@CBase@NHeroInformationScreens@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2E41, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 3 of NHeroInformationScreens::CBase's vtable 0x0123A3E0 (the slot holds this body in 7 vtable(s), owner NHeroInformationScreens::CBase); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts IsInventoryScreen(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsInventoryScreen@CBase@NHeroInformationScreens@@UBE_NXZ; `ret` matches the signature.
// Was: NHeroInformationScreens::CBase::vfunc_3 (rebuild/src/compiled/00/4d/gapscan_sub004d2e41_004d2e41.cpp).
#pragma optimize("s", on)
#include "engine/NHeroInformationScreens_CBase.h"
namespace NHeroInformationScreens {
bool CBase::IsInventoryScreen() const
{
    return true;
}
}
