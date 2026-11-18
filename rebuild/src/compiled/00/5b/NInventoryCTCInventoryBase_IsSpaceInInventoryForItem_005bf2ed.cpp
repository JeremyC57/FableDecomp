// NInventory::CTCInventoryBase::IsSpaceInInventoryForItem(long itemDefIndex) const -- whether the inventory can take one more of the item: finds
//   the category of the item's inventory category index and asks it (CCategory::IsSpaceInCategoryForItem 0x005BF19C, tail jump)
// Leaf: ?IsSpaceInInventoryForItem@CTCInventoryBase@NInventory@@QBE_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF2ED, 32 bytes, `jmp` (the callee's `ret 4` returns for it); size-optimised TU.
// Name: Ego_r.pdb 0x0062209A + FableWin.pdb ?IsSpaceInInventoryForItem@CTCInventoryBase@NInventory@@QBE_NJ@Z (the landed caller 0x00849700 binds
//   this spelling; the catalog's GetItemDescription label here is a propagated CItem name). Callee Ego_r.pdb + FableWin.pdb
//   ?IsSpaceInCategoryForItem@CCategory@NInventory@@QBE_NJ@Z @ 0x005BF19C.
// Callees: CTCInventoryItem::GetInventoryCategory 0x005D97B0 (static __fastcall), std::find_if<const CCategory*, CIsInventoryCategory>
//   0x005C08C0 over Categories (+0x20), CCategory::IsSpaceInCategoryForItem 0x005BF19C.
// Shape: as 0x005BEAD5 (byte twin): no end check; the found category is a named local, so the final call is a sibling `jmp`.
// Port (NF88, round 7, q11159 binder: it bound 0x005C08C0 as the free stand-in ?FindCategoryWithDefIndex@NInventory@@YIPAVCCategory@1@PAV21@0J@Z):
//   the local NInventory::CCategory / CTCInventoryBase / CCategoryVector models are gone: the row is the 0x005BEAD5 form (NInventory_CTCInventoryBase.h
//   with ALL_ITEMS_CHAIN_PASS22 + BUILDINGTHING, CCategory.h NF88 opt-in FABLE_RETAIL_CCATEGORY_ITEMQUERIES, CTCInventoryItem.h CATEGORY,
//   fable_algorithm.h find_if declared).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryItem.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
bool CTCInventoryBase::IsSpaceInInventoryForItem(long itemDefIndex) const
{
    const CCategory* category = std::find_if(Categories.begin(), Categories.end(),
                                             CIsInventoryCategory(CTCInventoryItem::GetInventoryCategory(itemDefIndex)));
    return category->IsSpaceInCategoryForItem(itemDefIndex);
}
}
