// NUISystem::CList::IsScrolling() const -- returns bit 4 of FlagsSetTwo
// Leaf: ?IsScrolling@CList@NUISystem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CE41, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 171 of NUISystem::CList's vtable 0x012462E4 (the slot holds this body in 1 vtable(s), owner NUISystem::CList); the Xbox 360 Test PDB virtual order (third source) aligned on 76 TLC-named slots of that vtable puts IsScrolling(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsScrolling@CList@NUISystem@@UBE_NXZ; `ret` matches the signature.
// Was: _unattributed::vfunc_171 (rebuild/src/compiled/00/53/unattributed_vfunc171_0053ce41.cpp).
#include "engine/NUISystem_CList.h"
namespace NUISystem {
bool CList::IsScrolling() const
{
    return (FlagsSetTwo >> 4) & 1;
}
}
