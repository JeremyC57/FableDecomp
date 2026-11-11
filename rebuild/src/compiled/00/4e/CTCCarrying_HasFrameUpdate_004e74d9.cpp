// CTCCarrying::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCarrying@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E74D9, thiscall, `ret`.
// Identity: slot 8 of CTCCarrying's vtable 0x012433F4 (defined here: the slot holds this body in 1 vtable(s), all of CTCCarrying or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCarrying@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCCarrying.h (opt-ins as in the landed row CTCHero_SetWeaponsAsUsable_006ca380.cpp).
#define FABLE_RETAIL_CTCCARRYING_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCCARRYING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"
bool CTCCarrying::HasFrameUpdate() const
{
    return true;
}
