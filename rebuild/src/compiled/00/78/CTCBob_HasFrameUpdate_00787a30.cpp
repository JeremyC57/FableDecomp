// CTCBob::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBob@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00787A30, thiscall, `ret`.
// Identity: slot 8 of CTCBob's vtable 0x0126B76C (defined here: the slot holds this body in 1 vtable(s), all of CTCBob or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBob@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBob.h (opt-ins as in the landed row CTCBob_SetForceVisible_00787740.cpp).
#define FABLE_RETAIL_CTCBOB_VSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBob.h"
bool CTCBob::HasFrameUpdate() const
{
    return true;
}
