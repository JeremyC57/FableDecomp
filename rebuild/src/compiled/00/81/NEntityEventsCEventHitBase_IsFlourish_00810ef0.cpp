// NEntityEvents::CEventHitBase::IsFlourish() const -- whether the hit was a flourish (the hit parameters' Flourish flag, +0x1D)
// Leaf: ?IsFlourish@CEventHitBase@NEntityEvents@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00810EF0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 5 of NEntityEvents::CEventHitBy's vtable 0x01272FA8 (the slot holds this body in 2 vtable(s), owner NEntityEvents::CEventHitBase); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts IsFlourish(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsFlourish@CEventHitBase@NEntityEvents@@UBE_NXZ; `ret` matches the signature.
// Was: NEntityEvents::CEventHitBase::vfunc_5 (rebuild/src/compiled/00/81/unattributed_vfunc5_00810ef0.cpp).
#define FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/NEntityEvents_CEventHitBase.h"
namespace NEntityEvents {
bool CEventHitBase::IsFlourish() const
{
    return PHitParams->IsFlourish();
}
}
