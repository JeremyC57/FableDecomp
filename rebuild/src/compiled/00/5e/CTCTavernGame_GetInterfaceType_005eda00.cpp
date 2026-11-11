// CTCTavernGame::GetInterfaceType() const -- returns TCI_TAVERN_GAME
// Leaf: ?GetInterfaceType@CTCTavernGame@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EDA00, thiscall, `ret`.
// Identity: slot 21 of CTCTavernGameCardBase's vtable 0x01288294 (defined here: the slot holds this body in 6 vtable(s), all of CTCTavernGame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 36 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot; Ego_r/FableWin name set: ?GetInterfaceType@CTCTavernGame@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
// Header: engine/CTCTavernGame.h (opt-ins as in the landed row CTCTavernGame_dynamicatexitdestructorfornotextentry_01229150.cpp).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/ETCInterfaceType.h"
ETCInterfaceType CTCTavernGame::GetInterfaceType() const
{
    return TCI_TAVERN_GAME;
}
