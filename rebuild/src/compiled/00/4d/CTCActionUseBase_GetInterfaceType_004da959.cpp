// CTCActionUseBase::GetInterfaceType() const -- the component's interface type, TCI_ON_ACTION_USE (0xD)
// Leaf: ?GetInterfaceType@CTCActionUseBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA959, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCActionUseBuyHouse's vtable 0x01266A7C (the slot holds this body in 21 vtable(s), owner CTCActionUseBase); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCActionUseBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCActionUseBase::vfunc_21 (rebuild/src/compiled/00/4d/CTCActionUseBase_vfunc21_004da959.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCActionUseBase.h"
ETCInterfaceType CTCActionUseBase::GetInterfaceType() const
{
    return StatGetInterfaceType();
}
