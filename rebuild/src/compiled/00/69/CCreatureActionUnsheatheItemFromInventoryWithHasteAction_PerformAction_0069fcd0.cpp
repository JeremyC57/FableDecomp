// CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::PerformAction() -- forwards to the base UnsheatheItemFromInventory::PerformAction
// Leaf: ?PerformAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069FCD0, 5 bytes: `jmp 0x0069F9E0` (this passed through in ecx).
// Identity: Ego_r.pdb 0x006C4FB0 + FableWin.pdb 0x02212790 ?PerformAction@CCreatureAction_UnsheatheItemFromInventoryWithHasteAction@@UAEXXZ;
//   retail vtable 0x0125CDDC (RTTI CCreatureAction_UnsheatheItemFromInventoryWithHasteAction) slot 3 = 0x0069FCD0, its only reference.
// Callee: ?PerformAction@CCreatureAction_UnsheatheItemFromInventory@@UAEXXZ 0x0069F9E0 (Ego_r 0x006C4C10 / FableWin 0x02211A10; the landed
//   self leaf of 0x0069F9E0), called qualified (non-virtual), which VC7.1 emits as the tail jump.
// NF270 (2026-11-22): moved onto the shared headers (engine/CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h, the define set of its
//   landed sibling 0x006A02B0, plus the add-only opt-in FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_PERFORMACTION). It was the July free-function
//   forwarder extern "C" CCreatureAction_UnsheatheItemFromInventoryWithHasteAction_PerformAction -> ForwardTarget_0069f9e0 (both placeholders).
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEHASTE_PERFORMACTION
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_UnsheatheItemFromInventoryWithHasteAction.h"
void CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::PerformAction()
{
    CCreatureAction_UnsheatheItemFromInventory::PerformAction();
}
