// NUISystem::CList::IsInverted() const -- returns bit 5 of FlagsSetTwo
// Leaf: ?IsInverted@CList@NUISystem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CE50, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 172 of NUISystem::CList's vtable 0x012462E4 (the slot holds this body in 1 vtable(s), owner NUISystem::CList); the Xbox 360 Test PDB virtual order (third source) aligned on 76 TLC-named slots of that vtable puts IsInverted(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsInverted@CList@NUISystem@@UBE_NXZ; `ret` matches the signature.
// Was: _unattributed::vfunc_172 (rebuild/src/compiled/00/53/unattributed_vfunc172_0053ce50.cpp).
#include "engine/NUISystem_CList.h"
namespace NUISystem {
bool CList::IsInverted() const
{
    return (FlagsSetTwo >> 5) & 1;
}
}
