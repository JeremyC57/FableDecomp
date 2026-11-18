// NInventory::CTCInventoryBase::GetPerItemDataForItem(long itemDefIndex) const -- the per-item data of the item: finds the category of the
//   item's inventory category index and asks it (CCategory::GetPerItemDataForItem 0x005BE8B1, tail jump)
// Leaf: ?GetPerItemDataForItem@CTCInventoryBase@NInventory@@QBEPBVCPerItemData@2@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE8D3, 32 bytes, `jmp` (the callee's `ret 4` returns for it); size-optimised TU.
// Name: Ego_r.pdb 0x00621169 ?GetPerItemDataForItem@CTCInventoryBase@NInventory@@QBEPBVCPerItemData@2@J@Z (TU order: CCategory::GetPerItemDataForItem
//   0x00621047, OnItemAdded, this, CreateInventoryObject 0x0062118F = retail 0x005BE8F3); the tail-jump target 0x005BE8B1 is
//   CCategory::GetPerItemDataForItem (q11156); the landed CreateInventoryObject 0x005BE8F3 and NUISystem::CManager::Action 0x0041E6D3 bind this
//   spelling (q11155). The catalog's CTCInventoryBase::GetBoughtPrice is wrong (that is 0x005BEAD5: one address, one name).
// Callees: CTCInventoryItem::GetInventoryCategory 0x005D97B0 (static __fastcall), std::find_if<const CCategory*, CIsInventoryCategory>
//   0x005C08C0 over Categories (+0x20), CCategory::GetPerItemDataForItem 0x005BE8B1.
// Shape: as 0x005BEAD5 (byte twin): no end check; the found category is a named local, so the final call is a sibling `jmp`.
// Port (NF88, round 7, q11155): the local CTCInventoryBase_Retail / CCategory_Retail / CCategoryVector models and the free FindCategoryWithDefIndex
//   stand-in are gone: the row is the 0x005BEAD5 form (NInventory_CTCInventoryBase.h with ALL_ITEMS_CHAIN_PASS22 + PERITEMDATA, CCategory.h
//   NF88 opt-in FABLE_RETAIL_CCATEGORY_ITEMQUERIES, CTCInventoryItem.h CATEGORY, fable_algorithm.h find_if declared).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryItem.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
const CPerItemData* CTCInventoryBase::GetPerItemDataForItem(long itemDefIndex) const
{
    const CCategory* category = std::find_if(Categories.begin(), Categories.end(),
                                             CIsInventoryCategory(CTCInventoryItem::GetInventoryCategory(itemDefIndex)));
    return category->GetPerItemDataForItem(itemDefIndex);
}
}
