// NUISystem::CComponent::GetChildren() -- returns Children
// Leaf: ?GetChildren@CComponent@NUISystem@@UAEAAV?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E7D0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 53 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetChildren(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetChildren@CComponent@NUISystem@@UAEAAV?$vector@V?$CCountedPointer@VCComponent@NUISystem@@@@V?$allocator@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@@std@@XZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_53 (rebuild/src/compiled/00/52/gapscan_sub0052e7d0_0052e7d0.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
std::vector<CCountedPointer<CComponent>, std::allocator<CCountedPointer<CComponent> > >& CComponent::GetChildren()
{
    return Children;
}
}
