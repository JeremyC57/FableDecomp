// CTCBoastingPosition::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBoastingPosition@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00782660, thiscall, `ret`.
// Identity: slot 8 of CTCBoastingPosition's vtable 0x0126B24C (defined here: the slot holds this body in 1 vtable(s), all of CTCBoastingPosition or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBoastingPosition@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBoastingPosition.h (opt-ins as in the landed row CTCBoastingPosition_IsPositionActivated_00781d80.cpp).
#define FABLE_RETAIL_CTCBOASTINGPOSITION_VSLOTS
#include "engine/CTCBoastingPosition.h"
bool CTCBoastingPosition::HasFrameUpdate() const
{
    return true;
}
