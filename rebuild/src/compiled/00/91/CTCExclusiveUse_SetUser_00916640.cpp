// CTCExclusiveUse::SetUser(CThing*) -- routes the argument through a temporary CIntelligentPointer<CThing> into User (+0x0C)
// Leaf: ?SetUser@CTCExclusiveUse@@QAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00916640.
// A stack CIntelligentPointer<CThing> (vtable 0x01238C6C, over CBaseIntelligentPointer() @ 0x00A01B10) is pointed at the argument
// with SetPItem @ 0x00A01B90, read back with GetPItem @ 0x00A01B50 into User (+0x0C) by SetPItem, and destroyed @ 0x00A01C10.
// NF39 (2026-11-16): moved off its address-suffixed stand-in (CClass_00916640::AssignTarget_00916640) onto the shared
// engine/CTCExclusiveUse.h (opt-in FABLE_RETAIL_CTCEXCLUSIVEUSE, which names 0x00916640): Ego_r.pdb 0x0080C470 + FableWin.pdb 0x017F33BB
// ?SetUser@CTCExclusiveUse@@QAEXPAVCThing@@@Z; Ego_r TPI CTCExclusiveUse::User (CIntelligentPointer<CThing>) +0x0C. Include chain from the
// landed 0x00915B60 PlaceGoods (the caller of this row).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCEXCLUSIVEUSE
#include "engine/CThing.h"
#include "engine/CTCExclusiveUse.h"

void CTCExclusiveUse::SetUser(CThing* user)
{
    User = user;
}
