// NUISystem::CObservable::AddExclusiveObserver(NUISystem::CObserver*) -- stores the exclusive observer
// Leaf: ?AddExclusiveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055C930, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 3 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 2 vtable(s), owner NUISystem::CObservable); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts AddExclusiveObserver(NUISystem::CObserver *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?AddExclusiveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z; `ret` matches the signature.
// Was: NUISystem::CObservable::vfunc_3 (rebuild/src/compiled/00/55/gapscan_sub0055c930_0055c930.cpp).
#include "engine/NUISystem_CObservable.h"
namespace NUISystem {
void CObservable::AddExclusiveObserver(CObserver* observer)
{
    m_pExclusiveObserver = observer;
}
}
