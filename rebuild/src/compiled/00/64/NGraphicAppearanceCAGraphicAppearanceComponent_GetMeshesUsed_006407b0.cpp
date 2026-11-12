// NGraphicAppearance::CAGraphicAppearanceComponent::GetMeshesUsed(std::vector<NGraphicAppearance::CMeshInfo,std::allocator<NGraphicAppearance::CMeshInfo>>&) const -- does nothing (the base appearance component uses no meshes)
// Leaf: ?GetMeshesUsed@CAGraphicAppearanceComponent@NGraphicAppearance@@UBEXAAV?$vector@VCMeshInfo@NGraphicAppearance@@V?$allocator@VCMeshInfo@NGraphicAppearance@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006407B0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 2 of NGraphicAppearance::CAGraphicAppearanceComponent's vtable 0x0125A170 (the slot holds this body in 4 vtable(s), owner NGraphicAppearance::CAGraphicAppearanceComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 0 TLC-named slots of that vtable puts GetMeshesUsed(std::vector<NGraphicAppearance::CMeshInfo,std::allocator<NGraphicAppearance::CMeshInfo> > &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetMeshesUsed@CAGraphicAppearanceComponent@NGraphicAppearance@@UBEXAAV?$vector@VCMeshInfo@NGraphicAppearance@@V?$allocator@VCMeshInfo@NGraphicAppearance@@@std@@@std@@@Z; `ret` matches the signature.
// Was: NGraphicAppearance::CAGraphicAppearanceComponent::vfunc_2 (rebuild/src/compiled/00/64/gapscan_sub006407b0_006407b0.cpp).
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
namespace NGraphicAppearance {
void CAGraphicAppearanceComponent::GetMeshesUsed(std::vector<CMeshInfo, std::allocator<CMeshInfo> >&) const
{
}
}
