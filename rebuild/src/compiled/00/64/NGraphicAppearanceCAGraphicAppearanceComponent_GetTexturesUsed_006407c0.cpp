// NGraphicAppearance::CAGraphicAppearanceComponent::GetTexturesUsed(std::vector<long,std::allocator<long>>&) const -- does nothing (the base appearance component uses no textures)
// Leaf: ?GetTexturesUsed@CAGraphicAppearanceComponent@NGraphicAppearance@@UBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006407C0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 3 of NGraphicAppearance::CAGraphicAppearanceComponent's vtable 0x0125A170 (the slot holds this body in 12 vtable(s), owner NGraphicAppearance::CAGraphicAppearanceComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetTexturesUsed(std::vector<long,std::allocator<long> > &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetTexturesUsed@CAGraphicAppearanceComponent@NGraphicAppearance@@UBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z; `ret` matches the signature.
// Was: NGraphicAppearance::CAGraphicAppearanceComponent::vfunc_3 (rebuild/src/compiled/00/64/gapscan_sub006407c0_006407c0.cpp).
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
namespace NGraphicAppearance {
void CAGraphicAppearanceComponent::GetTexturesUsed(std::vector<long, std::allocator<long> >&) const
{
}
}
