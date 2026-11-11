// CTCWallMount::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCWallMount@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DE490, thiscall, `ret`.
// Identity: slot 23 of CTCWallMount's vtable 0x0126FB7C (defined here: the slot holds this body in 1 vtable(s), all of CTCWallMount or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCWallMount@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCWallMount.h (opt-ins as in the landed row CTCWallMount_GetObjectAttached_007de4e0.cpp).
#define FABLE_RETAIL_CTCWALLMOUNT_VSLOTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CTCWallMount.h"
bool CTCWallMount::IsComponentSerialisable() const
{
    return true;
}
