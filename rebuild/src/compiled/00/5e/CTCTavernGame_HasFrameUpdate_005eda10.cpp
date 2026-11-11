// CTCTavernGame::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTavernGame@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EDA10, thiscall, `ret`.
// Identity: slot 8 of CTCTavernGameCardBase's vtable 0x01288294 (defined here: the slot holds this body in 9 vtable(s), all of CTCTavernGame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 36 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTavernGame@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCTavernGame.h (opt-ins as in the landed row CTCTavernGame_dynamicatexitdestructorfornotextentry_01229150.cpp).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
bool CTCTavernGame::HasFrameUpdate() const
{
    return true;
}
