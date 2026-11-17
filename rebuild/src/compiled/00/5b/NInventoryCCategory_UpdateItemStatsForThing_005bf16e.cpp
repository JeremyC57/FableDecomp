// NInventory::CCategory::UpdateItemStatsForThing(const CThing& thing) -- refresh the item stats of the slot holding the thing's item type
// Leaf: ?UpdateItemStatsForThing@CCategory@NInventory@@QAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF16E, 46 bytes, `ret 4`; size-optimised TU.
// Name: Ego_r.pdb 0x00621F4C + FableWin.pdb ?UpdateItemStatsForThing@CCategory@NInventory@@QAEXABVCThing@@@Z (public), between AddInventoryItem
//   0x00621EE3 (retail 0x005BF103) and IsSpaceInCategoryForItem 0x00621F79 (retail 0x005BF19C); the catalog already names it.
// Callees: CCategory::GetSlotWithItemsOfType 0x005BE85B (Ego_r.pdb private const), CItem::UpdateItemStats 0x005BEDCB (Ego_r.pdb / FableWin.pdb
//   ?UpdateItemStats@CItem@NInventory@@QAEXABVCThing@@@Z); the type is CThing::GetDefGlobalIndex (FableWin.pdb, inline: the word at +0x8C).
// Port (NF90, q11182 binder): was the local CCategory_Methods / CItemCalls model over a byte-offset read (leaf
//   ?UpdateItemStatsForThing@CCategory_Methods@@QAEXPAVCThing@@@Z, binding 0x005BE85B as ?GetSlotWithItemsOfType@CCategory_Methods@@QAEJJ@Z);
//   now on engine/NInventory_CTCInventoryBase.h (CONFISCATED CItem + NF90 opt-in FABLE_RETAIL_NINVENTORY_CITEM_UPDATEITEMSTATS) + engine/CCategory.h
//   (PASS22, NF88 _ITEMQUERIES, NF90 opt-in FABLE_RETAIL_CCATEGORY_UPDATEITEMSTATSFORTHING) + engine/CThing.h.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CCATEGORY_UPDATEITEMSTATSFORTHING
#define FABLE_RETAIL_NINVENTORY_CITEM_UPDATEITEMSTATS
#include "engine/CThing.h"
#include "engine/NInventory_CTCInventoryBase.h"

namespace NInventory {
void CCategory::UpdateItemStatsForThing(const CThing& thing)
{
    long slot = GetSlotWithItemsOfType(thing.GetDefGlobalIndex());
    if (slot != -1)
        ItemsInCategory[slot].UpdateItemStats(thing);
}
}
