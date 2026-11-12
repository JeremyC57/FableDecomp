// CTCContainer::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCContainer@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074FCA0, thiscall, `ret`.
// Identity: slot 23 of CTCContainerRewardHero's vtable 0x01269424 (defined here: the slot holds this body in 2 vtable(s), all of CTCContainer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCContainer@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCContainer.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_CTCCONTAINER_VSLOTS
#include "engine/CTCContainer.h"
bool CTCContainer::IsComponentSerialisable() const
{
    return true;
}
