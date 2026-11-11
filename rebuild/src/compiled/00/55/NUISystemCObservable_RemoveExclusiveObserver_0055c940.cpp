// NUISystem::CObservable::RemoveExclusiveObserver(NUISystem::CObserver*) -- clears the exclusive observer (the argument is unused)
// Leaf: ?RemoveExclusiveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C940, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 2 vtable(s), owner NUISystem::CObservable); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts RemoveExclusiveObserver(NUISystem::CObserver *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?RemoveExclusiveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z; `ret` matches the signature.
// Was: _unattributed::vfunc_4 (rebuild/src/compiled/00/55/unattributed_vfunc4_0055c940.cpp).
#include "engine/NUISystem_CObservable.h"
namespace NUISystem {
void CObservable::RemoveExclusiveObserver(CObserver*)
{
    m_pExclusiveObserver = 0;
}
}
