// CTCHeroCentre::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHeroCentre@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077EB90, thiscall, `ret`.
// Identity: slot 8 of CTCHeroCentre's vtable 0x0126AF94 (defined here: the slot holds this body in 1 vtable(s), all of CTCHeroCentre or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHeroCentre@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHeroCentre.h (opt-ins as in the landed row (bare include)).
#define FABLE_RETAIL_CTCHEROCENTRE_VSLOTS
#include "engine/CTCHeroCentre.h"
bool CTCHeroCentre::HasFrameUpdate() const
{
    return true;
}
