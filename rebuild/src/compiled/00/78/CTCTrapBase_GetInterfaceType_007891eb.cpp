// CTCTrapBase::GetInterfaceType() const -- the component's interface type, TCI_TRAP (0x7D)
// Leaf: ?GetInterfaceType@CTCTrapBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007891EB, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCProjectileTrap's vtable 0x0126B90C (the slot holds this body in 3 vtable(s), owner CTCTrapBase); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCTrapBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCTrapBase::vfunc_21 (rebuild/src/compiled/00/78/CTCTrapBase_vfunc21_007891eb.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTrapBase.h"
ETCInterfaceType CTCTrapBase::GetInterfaceType() const
{
    return StatGetInterfaceType();
}
