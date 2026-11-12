// CTCDoor::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDoor@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007312E0, thiscall, `ret`.
// Identity: slot 8 of CTCDoor's vtable 0x01266DE4 (defined here: the slot holds this body in 1 vtable(s), all of CTCDoor or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDoor@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCDoor.h (opt-ins as in the landed row CTCDoor_CTCDoor_00731330.cpp).
#define FABLE_RETAIL_CTCDOOR_VSLOTS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCDOOR
#include "engine/CTCDoor.h"                              // retail branch: CTCDoor : CTCBase, vtable 0x01266DE4
bool CTCDoor::HasFrameUpdate() const
{
    return true;
}
