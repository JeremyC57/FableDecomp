// CTCCarrying::FrameUpdate() -- does nothing
// Leaf: ?FrameUpdate@CTCCarrying@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A0FE0, thiscall, `ret`.
// Identity: slot 9 of CTCCarrying's vtable 0x012433F4 (defined here: the slot holds this body in 1 vtable(s), all of CTCCarrying or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts FrameUpdate(void) at this slot; Ego_r/FableWin name set: ?FrameUpdate@CTCCarrying@@UAEXXZ; `ret` matches the signature.
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
void CTCCarrying::FrameUpdate()
{
}
