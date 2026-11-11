// CTCWeapon::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWeapon@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DAE10, thiscall, `ret`.
// Identity: slot 8 of CTCWeapon's vtable 0x012554FC (defined here: the slot holds this body in 1 vtable(s), all of CTCWeapon or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWeapon@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCWeapon.h (opt-ins as in the landed row CTCWeapon_IsAbleToBeUsedToBlock_005dacf0.cpp).
#define FABLE_RETAIL_CTCWEAPON_VSLOTS
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_BLOCK
#include "engine/CTCWeapon.h"
bool CTCWeapon::HasFrameUpdate() const
{
    return true;
}
