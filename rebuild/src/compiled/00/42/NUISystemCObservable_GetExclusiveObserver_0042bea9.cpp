// NUISystem::CObservable::GetExclusiveObserver() -- returns m_pExclusiveObserver
// Leaf: ?GetExclusiveObserver@CObservable@NUISystem@@UAEPAVCObserver@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042BEA9, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 7 of NUISystem::CManager's vtable 0x01230134 (the slot holds this body in 2 vtable(s), owner NUISystem::CObservable); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts GetExclusiveObserver(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetExclusiveObserver@CObservable@NUISystem@@UAEPAVCObserver@2@XZ; `ret` matches the signature.
// Was: NUISystem::CObservable::vfunc_7 (rebuild/src/compiled/00/42/gapscan_sub0042bea9_0042bea9.cpp).
#include "engine/NUISystem_CObservable.h"
namespace NUISystem {
CObserver* CObservable::GetExclusiveObserver()
{
    return m_pExclusiveObserver;
}
}
