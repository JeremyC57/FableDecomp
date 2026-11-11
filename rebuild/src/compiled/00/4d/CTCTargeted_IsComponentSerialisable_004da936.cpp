// CTCTargeted::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCTargeted@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA936, thiscall, `ret`.
// Identity: slot 23 of CTCTargeted's vtable 0x0123F024 (defined here: the slot holds this body in 1 vtable(s), all of CTCTargeted or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCTargeted@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCTargeted.h (opt-ins as in the landed row CTCWillResponse_IsThingLightningable_007e4530.cpp).
#define FABLE_RETAIL_CTCTARGETED_VSLOTS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#define FABLE_RETAIL_CTCTARGETED
#include "engine/CTCTargeted.h"
bool CTCTargeted::IsComponentSerialisable() const
{
    return true;
}
