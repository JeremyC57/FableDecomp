// CTCDNavigationSeed::IsComponentSerialisable() const -- driver components are serialisable
// Leaf: ?IsComponentSerialisable@CTCDNavigationSeed@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D422F, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 23 of CTCDNavigationSeed's vtable 0x0123B944 (the slot holds this body in 1 vtable(s), owner CTCDNavigationSeed); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCDNavigationSeed@@UBE_NXZ; `ret` matches the signature.
// Was: CTCDNavigationSeed::vfunc_23 (rebuild/src/compiled/00/4d/gapscan_sub004d422f_004d422f.cpp).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCDNAVIGATIONSEED_VSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCDNavigationSeed.h"
bool CTCDNavigationSeed::IsComponentSerialisable() const
{
    return true;
}
