// NInventory::CTCInventoryBase::GetNumberOfSlotsForCategory(long index) const -- the slot count of the index-th inventory category (Categories[index].GetNumberOfSlots())
// Leaf: ?GetNumberOfSlotsForCategory@CTCInventoryBase@NInventory@@QBEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD328, length 20 bytes, thiscall `ret 4`; size-optimised TU, the result passes straight through eax.
// Callee: NInventory::CCategory::GetNumberOfSlots @ 0x005BD1CF. Caller: CTCInventoryExperience::ResetStatIndicatorValues @ 0x0056E5AB (on `this`, with
//   GetSelectedCategoryIndex()).
// Name fix (agentNF89, q11167 follow-through): Ego_r.pdb (0x00620259) + FableWin.pdb ?GetNumberOfSlotsForCategory@CTCInventoryBase@NInventory@@QBEJJ@Z,
//   the CTCInventoryBase (long) -> long member that sits in the same Ego_r TU order as its neighbours (GetCategoryThingAssociation 0x006200E1 ->
//   0x005BD1AE, CCategory::GetNumberOfSlots 0x00620102 -> 0x005BD1CF); was the descriptive GetNumberOfSlotsInCategory (QAEJJ) on a row-local
//   CTCInventoryBase / CCategory model. Now on the header's PASS22 chain (overlay opt-ins CTCINVENTORYBASE_NUMBEROFSLOTSFORCATEGORY,
//   CCATEGORY_NUMBEROFSLOTS).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_NUMBEROFSLOTSFORCATEGORY
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_NUMBEROFSLOTS
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
long CTCInventoryBase::GetNumberOfSlotsForCategory(long index) const
{
    return Categories[index].GetNumberOfSlots();
}
}
