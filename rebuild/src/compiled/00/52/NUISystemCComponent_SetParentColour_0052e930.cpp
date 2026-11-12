// NUISystem::CComponent::SetParentColour(CRGBColour&) -- stores the parent colour
// Leaf: ?SetParentColour@CComponent@NUISystem@@UAEXAAVCRGBColour@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E930, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 22 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts SetParentColour(CRGBColour &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetParentColour@CComponent@NUISystem@@UAEXAAVCRGBColour@@@Z; `ret` matches the signature.
// Was: _unattributed::vfunc_22 (rebuild/src/compiled/00/52/unattributed_vfunc22_0052e930.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
void CComponent::SetParentColour(CRGBColour& colour)
{
    ParentColour = colour;
}
}
