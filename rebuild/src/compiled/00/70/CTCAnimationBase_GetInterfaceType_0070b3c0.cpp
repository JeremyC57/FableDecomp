// CTCAnimationBase::GetInterfaceType() const -- the component's interface type, TCI_ANIMATION (0x5A)
// Leaf: ?GetInterfaceType@CTCAnimationBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070B3C0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 21 of CTCAnimationComplex's vtable 0x012650A4 (the slot holds this body in 2 vtable(s), owner CTCAnimationBase); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetInterfaceType@CTCAnimationBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Was: CTCAnimationBase::vfunc_21 (rebuild/src/compiled/00/70/gapscan_sub0070b3c0_0070b3c0.cpp).
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAnimationBase.h"
ETCInterfaceType CTCAnimationBase::GetInterfaceType() const
{
    return StatGetInterfaceType();
}
