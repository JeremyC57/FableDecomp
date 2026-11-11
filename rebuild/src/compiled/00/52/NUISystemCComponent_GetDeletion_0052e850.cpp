// NUISystem::CComponent::GetDeletion() -- returns Deletion (the header keeps the 8-byte CDeletion member opaque: engine/NUISystem_CDeletion.h includes this header)
// Leaf: ?GetDeletion@CComponent@NUISystem@@UAEAAVCDeletion@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E850, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 63 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 43 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetDeletion(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetDeletion@CComponent@NUISystem@@UAEAAVCDeletion@2@XZ; `ret` matches the signature.
// Was: NUISystem::CSprite::vfunc_63 (rebuild/src/compiled/00/52/gapscan_sub0052e850_0052e850.cpp).
#include "engine/NUISystem_CDeletion.h"
namespace NUISystem {
CDeletion& CComponent::GetDeletion()
{
    return *reinterpret_cast<CDeletion*>(Deletion);
}
}
