// NGraphicAppearance::CAGraphicAppearanceComponent::IsAppearanceSentEveryRenderFrame() -- returns false
// Leaf: ?IsAppearanceSentEveryRenderFrame@CAGraphicAppearanceComponent@NGraphicAppearance@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006407D0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NGraphicAppearance::CAGraphicAppearanceComponent's vtable 0x0125A170 (the slot holds this body in 11 vtable(s), owner NGraphicAppearance::CAGraphicAppearanceComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts IsAppearanceSentEveryRenderFrame(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsAppearanceSentEveryRenderFrame@CAGraphicAppearanceComponent@NGraphicAppearance@@UAE_NXZ; `ret` matches the signature.
// Was: NGraphicAppearance::CAGraphicAppearanceComponent::vfunc_4 (rebuild/src/compiled/00/64/gapscan_sub006407d0_006407d0.cpp).
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
namespace NGraphicAppearance {
bool CAGraphicAppearanceComponent::IsAppearanceSentEveryRenderFrame()
{
    return false;
}
}
