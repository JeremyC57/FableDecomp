// NInventory::CTCInventoryBase::GetAmountOfItemsContainable(long itemDefIndex) const -- how many more of the item the inventory can take: finds
//   the category of the item's inventory category index and asks it (CCategory::GetAmountOfItemsContainable 0x005BF1D7, tail jump)
// Leaf: ?GetAmountOfItemsContainable@CTCInventoryBase@NInventory@@QBEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF30D, 32 bytes, `jmp` (the callee's `ret 4` returns for it); size-optimised TU.
// Name: Ego_r.pdb 0x006220C0 ?GetAmountOfItemsContainable@CTCInventoryBase@NInventory@@QBEJJ@Z, right after IsSpaceInInventoryForItem 0x0062209A
//   (retail 0x005BF2ED, 0x20 earlier); the tail-jump target 0x005BF1D7 is CCategory::GetAmountOfItemsContainable (q11158); the landed
//   CTCInventoryTrade::ConstructBuyActionsList(PC) 0x005A3E47 / 0x005A79E6 bind this spelling (q11154). The catalog's GetItemDescription is a
//   propagated label.
// Callees: CTCInventoryItem::GetInventoryCategory 0x005D97B0 (static __fastcall), std::find_if<const CCategory*, CIsInventoryCategory>
//   0x005C08C0 over Categories (+0x20), CCategory::GetAmountOfItemsContainable 0x005BF1D7.
// Shape: as 0x005BEAD5 (byte twin): no end check; the found category is a named local, so the final call is a sibling `jmp`.
// Port (NF88, round 7, q11154): the free CTCInventoryBase_GetItemDescription / sub_5D97B0 / sub_5C08C0 / CFinal stand-ins are gone: the row is
//   the 0x005BEAD5 form (NInventory_CTCInventoryBase.h with ALL_ITEMS_CHAIN_PASS22 + CONTAINABLE, CCategory.h NF88 opt-in
//   FABLE_RETAIL_CCATEGORY_ITEMQUERIES, CTCInventoryItem.h CATEGORY, fable_algorithm.h find_if declared).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_CONTAINABLE
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryItem.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
long CTCInventoryBase::GetAmountOfItemsContainable(long itemDefIndex) const
{
    const CCategory* category = std::find_if(Categories.begin(), Categories.end(),
                                             CIsInventoryCategory(CTCInventoryItem::GetInventoryCategory(itemDefIndex)));
    return category->GetAmountOfItemsContainable(itemDefIndex);
}
}
