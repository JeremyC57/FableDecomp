// NUISystem::CComponent::GetStates() -- returns States (the header keeps the 12-byte STLport map member opaque)
// Leaf: ?GetStates@CComponent@NUISystem@@UAEAAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F0E0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 82 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetStates(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetStates@CComponent@NUISystem@@UAEAAV?$map@KVCUIState@NUISystem@@U?$less@K@std@@V?$allocator@U?$pair@$$CBKVCUIState@NUISystem@@@std@@@4@@std@@XZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_82 (rebuild/src/compiled/00/52/gapscan_sub0052f0e0_0052f0e0.cpp).
#include "stlport/fable_map.h"
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
std::map<unsigned long, CUIState, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CUIState> > >& CComponent::GetStates()
{
    return *reinterpret_cast<std::map<unsigned long, CUIState, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, CUIState> > >*>(States);
}
}
