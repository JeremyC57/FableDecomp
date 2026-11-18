// NInventory::CTCInventoryBase::GetBoughtPrice(long itemDefIndex) const -- the average bought price of the item: finds the category of the
//   item's inventory category index and asks it (CCategory::GetBoughtPrice 0x005BE839, tail jump)
// Leaf: ?GetBoughtPrice@CTCInventoryBase@NInventory@@QBEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BEAD5, 32 bytes, `jmp` (the callee's `ret 4` returns for it); size-optimised TU.
// Name: Ego_r.pdb 0x00621385 ?GetBoughtPrice@CTCInventoryBase@NInventory@@QBEJJ@Z, in the TU order CreateInventoryObject 0x0062118F ->
//   GetBoughtPrice -> GetAllSelectedItems 0x006213AB = retail 0x005BE8F3 -> 0x005BEAD5 -> 0x005BEAF5; the tail-jump target 0x005BE839 is
//   CCategory::GetBoughtPrice (q11157); the landed CTCInventoryTrade callers 0x005A9CBE / 0x005AAF28 / 0x005AD571 bind this spelling (q11153).
//   The catalog's GetItemDescription is a propagated label.
// Callees: CTCInventoryItem::GetInventoryCategory 0x005D97B0 (static __fastcall), std::find_if<const CCategory*, CIsInventoryCategory>
//   0x005C08C0 over Categories (+0x20), CCategory::GetBoughtPrice 0x005BE839.
// Shape: retail does not compare the found category against end; the found category is a named local, so the final call is a sibling `jmp`
//   that reuses the incoming argument slot.
// Port (NF88, round 7, q11153): the local CTCInventoryBase / Tgt5be839 / Sub20 / CWideString stand-ins are gone: the row is on
//   engine/NInventory_CTCInventoryBase.h (Categories typed by FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22 in the default branch) +
//   engine/CCategory.h (the PASS22 NInventory::CCategory, NF88 opt-in FABLE_RETAIL_CCATEGORY_ITEMQUERIES) + engine/CTCInventoryItem.h (CATEGORY)
//   + stlport/fable_algorithm.h (find_if declared).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryItem.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
long CTCInventoryBase::GetBoughtPrice(long itemDefIndex) const
{
    const CCategory* category = std::find_if(Categories.begin(), Categories.end(),
                                             CIsInventoryCategory(CTCInventoryItem::GetInventoryCategory(itemDefIndex)));
    return category->GetBoughtPrice(itemDefIndex);
}
}
