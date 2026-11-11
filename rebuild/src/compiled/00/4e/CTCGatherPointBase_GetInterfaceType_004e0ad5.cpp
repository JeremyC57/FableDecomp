// CTCGatherPointBase::GetInterfaceType() const -- the component's interface type TCI_GATHER_POINT (0x7B)
// Leaf: ?GetInterfaceType@CTCGatherPointBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E0AD5, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCGatherPointRadialArea's vtable 0x012422E4 (the slot holds this body in 3 vtable(s), owner CTCGatherPointBase); the Xbox 360 Test PDB virtual order (third source) aligned on 12 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCGatherPointBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCGatherPointBase::vfunc_21 (rebuild/src/compiled/00/4e/CTCGatherPointBase_vfunc21_004e0ad5.cpp).
#pragma optimize("s", on)
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/ETCInterfaceType.h"
#include "engine/CTCGatherPointBase.h"
ETCInterfaceType CTCGatherPointBase::GetInterfaceType() const
{
    return TCI_GATHER_POINT;
}
