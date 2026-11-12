// NUISystem::CComponent::IsIndependent() const -- returns bit 6 of Flags (independent of the parent)
// Leaf: ?IsIndependent@CComponent@NUISystem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052F190, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 101 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts IsIndependent(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsIndependent@CComponent@NUISystem@@UBE_NXZ; `ret` matches the signature.
// Was: _unattributed::vfunc_101 (rebuild/src/compiled/00/52/unattributed_vfunc101_0052f190.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
bool CComponent::IsIndependent() const
{
    return (Flags >> 6) & 1;
}
}
