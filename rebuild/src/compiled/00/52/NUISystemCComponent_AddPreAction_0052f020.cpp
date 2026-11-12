// NUISystem::CComponent::AddPreAction(CCountedPointer<NUISystem::CAction>&) -- does nothing (the base component keeps no actions)
// Leaf: ?AddPreAction@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F020, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 69 of NUISystem::CFrontEndScreen's vtable 0x012497E4 (the slot holds this body in 42 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 65 TLC-named slots of that vtable puts AddPreAction(CCountedPointer<NUISystem::CAction> &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?AddPreAction@CComponent@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_69 (rebuild/src/compiled/00/52/gapscan_sub0052f020_0052f020.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
void CComponent::AddPreAction(CCountedPointer<CAction>&)
{
}
}
