// CTCAnimationBase::AnimsCanBeIgnored() const -- animations cannot be ignored by default
// Leaf: ?AnimsCanBeIgnored@CTCAnimationBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070B3D0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 30 of CTCAnimationComplex's vtable 0x012650A4 (the slot holds this body in 2 vtable(s), owner CTCAnimationBase); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts AnimsCanBeIgnored(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?AnimsCanBeIgnored@CTCAnimationBase@@UBE_NXZ; `ret` matches the signature.
// Was: CTCAnimationBase::vfunc_30 (rebuild/src/compiled/00/70/gapscan_sub0070b3d0_0070b3d0.cpp).
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAnimationBase.h"
bool CTCAnimationBase::AnimsCanBeIgnored() const
{
    return false;
}
