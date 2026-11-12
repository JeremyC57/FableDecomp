// NUISystem::CComponent::GetPosition() const -- returns Position
// Leaf: ?GetPosition@CComponent@NUISystem@@UBE?AVC2DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E890, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 16 of NUISystem::CMorphingSprite's vtable 0x0124A49C (the slot holds this body in 42 vtable(s), owner NUISystem::CComponent); the Xbox 360 Test PDB virtual order (third source) aligned on 75 TLC-named slots of that vtable puts GetPosition(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetPosition@CComponent@NUISystem@@UBE?AVC2DVector@@XZ; `ret` matches the signature.
// Was: NUISystem::CComponent::vfunc_16 (rebuild/src/compiled/00/52/CMessageEventFilterIsOriginatedByThing_IsOriginatedByThing_0052e890.cpp).
#include "engine/NUISystem_CComponent.h"
namespace NUISystem {
C2DVector CComponent::GetPosition() const
{
    return Position;
}
}
