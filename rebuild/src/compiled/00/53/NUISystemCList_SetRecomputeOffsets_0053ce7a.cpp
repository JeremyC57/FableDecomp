// NUISystem::CList::SetRecomputeOffsets(bool) -- stores RecomputeOffsets
// Leaf: ?SetRecomputeOffsets@CList@NUISystem@@MAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CE7A, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 181 of NUISystem::CList's vtable 0x012462E4 (the slot holds this body in 1 vtable(s), owner NUISystem::CList); the Xbox 360 Test PDB virtual order (third source) aligned on 76 TLC-named slots of that vtable puts SetRecomputeOffsets(bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetRecomputeOffsets@CList@NUISystem@@MAEX_N@Z; `ret` matches the signature.
// Was: NUISystem::CList::vfunc_181 (rebuild/src/compiled/00/53/gapscan_sub0053ce7a_0053ce7a.cpp).
#define FABLE_RETAIL_CLIST
#include "engine/NUISystem_CList.h"
namespace NUISystem {
void CList::SetRecomputeOffsets(bool recompute)
{
    RecomputeOffsets = recompute;
}
}
