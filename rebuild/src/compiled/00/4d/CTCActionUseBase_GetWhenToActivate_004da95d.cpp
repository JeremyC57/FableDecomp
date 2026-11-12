// CTCActionUseBase::GetWhenToActivate() const -- when the action use activates: 1
// Leaf: ?GetWhenToActivate@CTCActionUseBase@@UBE?AW4EWhenToActivate@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA95D, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 29 of CTCCarriedActionUseBase's vtable 0x0123F894 (the slot holds this body in 40 vtable(s), owner CTCActionUseBase); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts GetWhenToActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetWhenToActivate@CTCActionUseBase@@UBE?AW4EWhenToActivate@@XZ; `ret` matches the signature.
// Was: CTCActionUseBase::vfunc_29 (rebuild/src/compiled/00/4d/CTCActionUseBase_vfunc29_004da95d.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCActionUseBase.h"
EWhenToActivate CTCActionUseBase::GetWhenToActivate() const
{
    return (EWhenToActivate)1;
}
