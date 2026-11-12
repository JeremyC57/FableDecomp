// NUISystem::CSwappingStateComponent::IsRandomSwap() const -- returns bit 3 of FlagsSetTwo
// Leaf: ?IsRandomSwap@CSwappingStateComponent@NUISystem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547970, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 142 of NUISystem::CSwappingStateComponent's vtable 0x012485AC (the slot holds this body in 1 vtable(s), owner NUISystem::CSwappingStateComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 64 TLC-named slots of that vtable puts IsRandomSwap(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsRandomSwap@CSwappingStateComponent@NUISystem@@UBE_NXZ; `ret` matches the signature.
// Was: _unattributed::vfunc_142 (rebuild/src/compiled/00/54/unattributed_vfunc142_00547970.cpp).
#define FABLE_RETAIL_CSWAPPINGSTATECOMPONENT_VSLOTS
#include "engine/NUISystem_CSwappingStateComponent.h"
namespace NUISystem {
bool CSwappingStateComponent::IsRandomSwap() const
{
    return (FlagsSetTwo >> 3) & 1;
}
}
