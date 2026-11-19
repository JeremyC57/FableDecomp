// NInventory::CTCInventoryBase::RemoveItemFromInventory(long itemDefIndex, long count) -- removes `count` items of `itemDefIndex` from the
//   category that holds that item type
// Leaf: ?RemoveItemFromInventory@CTCInventoryBase@NInventory@@UAEXJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF3C6, length 43 bytes; size TU. Virtual slot 79 (+0x13C) of the inventory
// screens (CTCInventoryBase vtable 0x01253DBC + 0x13C = 0x01253EF8 holds it); the catalog label "GetItemDescription" is propagated.
// Identity: Ego_r.pdb (0x00622188) + FableWin.pdb ?RemoveItemFromInventory@CTCInventoryBase@NInventory@@UAEXJJ@Z (public virtual), the
//   slot-79 declaration of engine/NInventory_CTCInventoryBase.h (FABLE_RETAIL_CTCINVENTORYBASE_VTABLE).
// Callees: CTCInventoryItem::GetInventoryCategory 0x005D97B0 (static __fastcall), std::find_if<CCategory*, CIsInventoryCategory> 0x005C08D3
//   (STLport, /Gr), NInventory::CCategory::RemoveItem(long, long) 0x005BF221 (Ego_r.pdb / FableWin.pdb).
// Shape: RemoveItem's two pass-through pushes sit between the GetInventoryCategory call and the find call, so the category def index is a
//   named local computed first and the find result feeds the member call directly.
// Port (NF93, q11239): was a local CTCInventoryBase_Retail / CCategory_Retail model with the find_if stand-in
//   FindCategoryWithDefIndex_005c08d3(first, last, long); now on engine/NInventory_CTCInventoryBase.h + engine/CCategory.h (opt-in
//   FABLE_RETAIL_CCATEGORY_REMOVEITEM, NF94's) and stlport/fable_algorithm.h, the AddInventoryItem 0x005BF4A3 recipe.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CCATEGORY_REMOVEITEM
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_CALLED_PASS23
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryItem.h"
#include "stlport/fable_algorithm.h"

namespace NInventory {
void CTCInventoryBase::RemoveItemFromInventory(long itemDefIndex, long count)
{
    long category_def_index = CTCInventoryItem::GetInventoryCategory(itemDefIndex);
    std::find_if(Categories.begin(), Categories.end(), CIsInventoryCategory(category_def_index))->RemoveItem(itemDefIndex, count);
}
}
