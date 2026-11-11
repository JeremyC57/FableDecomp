// CTCBerserk::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBerserk@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078DAF0, thiscall, `ret`.
// Identity: slot 8 of CTCBerserk's vtable 0x0126BB94 (defined here: the slot holds this body in 1 vtable(s), all of CTCBerserk or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBerserk@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBerserk.h (opt-ins as in the landed row CTCBerserk_SetLevel_0078bf00.cpp).
#define FABLE_RETAIL_CTCBERSERK_VSLOTS
#include "engine/CTCBerserk.h"
#include "engine/GFFloatToLongCeil.h"
bool CTCBerserk::HasFrameUpdate() const
{
    return true;
}
