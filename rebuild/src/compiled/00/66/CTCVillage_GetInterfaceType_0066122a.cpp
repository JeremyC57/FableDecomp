// CTCVillage::GetInterfaceType() const -- the component's interface type, TCI_VILLAGE (0x22)
// Leaf: ?GetInterfaceType@CTCVillage@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066122A, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCVillage's vtable 0x0125A9AC (the slot holds this body in 1 vtable(s), owner CTCVillage); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCVillage@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCVillage::vfunc_21 (rebuild/src/compiled/00/66/CTCVillage_vfunc21_0066122a.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCVILLAGE_VSLOTS3
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TCI
#include "engine/ETCInterfaceType.h"
#include "engine/CTCVillage.h"
ETCInterfaceType CTCVillage::GetInterfaceType() const
{
    return StatGetInterfaceType();
}
