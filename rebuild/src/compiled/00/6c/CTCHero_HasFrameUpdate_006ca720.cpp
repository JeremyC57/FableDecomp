// CTCHero::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHero@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CA720, thiscall, `ret`.
// Identity: slot 8 of CTCHero's vtable 0x0125EB54 (defined here: the slot holds this body in 1 vtable(s), all of CTCHero or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHero@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHero.h (opt-ins as in the landed row CTCHero_FrameUpdate_006c9fd0.cpp).
#define FABLE_RETAIL_CTCHERO_VSLOTS
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCHero.h"
#include "engine/CUserProfileManager.h"
bool CTCHero::HasFrameUpdate() const
{
    return true;
}
