// NUISystem::CComponent::SetRenderColour(CRGBColour&) -- stores the render colour
// Leaf: ?SetRenderColour@CComponent@NUISystem@@UAEXAAVCRGBColour@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E9B0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 30 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts SetRenderColour(CRGBColour &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetRenderColour@CComponent@NUISystem@@UAEXAAVCRGBColour@@@Z; `ret` matches the signature.
// Was: NUISystem::CComponent::vfunc_30 (rebuild/src/compiled/00/52/unattributed_vfunc30_0052e9b0.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
void CComponent::SetRenderColour(CRGBColour& colour)
{
    RenderColour = colour;
}
}
