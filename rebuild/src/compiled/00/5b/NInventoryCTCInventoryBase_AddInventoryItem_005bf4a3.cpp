// NInventory::CTCInventoryBase::AddInventoryItem(const NInventory::CItem& item) -- find the category whose def index is the item's inventory
//   category and add the item to it; on success notify the inventory (OnItemAdded(type, true, true)) and return true
// Leaf: ?AddInventoryItem@CTCInventoryBase@NInventory@@AAE_NABVCItem@2@@Z
// Retail: Fable.exe @ 0x005BF4A3, 74 bytes, `ret 4` (thiscall); size TU.
// Callers: CTCInventoryBase::InitialiseInventoryWithItems 0x005BF8CB, ReturnAllConfiscatedItems 0x005BF91A, Copy 0x005BFB96.
// Callees: CTCInventoryItem::GetInventoryCategory(long) 0x005D97B0 (static __fastcall), std::find_if<CCategory*, CIsInventoryCategory>
//   0x005C08D3 (__fastcall, over Categories +0x20), NInventory::CCategory::AddInventoryItem(const CItem&) 0x005BF103 (was bound as the
//   made-up ?AddItem@CCategory@NInventory@@QAE_NABUCItem@@@Z), virtual slot 84 OnItemAdded (+0x150).
// Shape: the category end is read into a local AFTER the category lookup (edi, compared with the find result); the `&&` of the two tests
//   shares the `xor al,al` tail with `mov al,1` after the virtual call.
// Identity (NF92, q11216): Ego_r.pdb 0x00622247 + FableWin.pdb 0x0236BE37 ?AddInventoryItem@CTCInventoryBase@NInventory@@AAE_NABVCItem@2@@Z (private, bool, const
//   CItem&; the PDB member record in engine/NInventory_CTCInventoryBase.h ORIGINAL_COMPLETE_METHODS_PASS22 lists it) +
//   the body, which adds the item through CCategory::AddInventoryItem. Was named by effect (catalog NInventory::CTCInventoryBase::AddItem, a
//   local CTCInventoryBase / CCategory / CItem / find_if model); now on engine/NInventory_CTCInventoryBase.h (VTABLE, NF92 opt-in
//   ADDINVENTORYITEM), CCategory.h (NF90 _ADDINVENTORYITEM) and fable_algorithm.h.
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
#define FABLE_RETAIL_CTCINVENTORYBASE_ADDINVENTORYITEM
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_CCATEGORY_ADDINVENTORYITEM
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
bool CTCInventoryBase::AddInventoryItem(const CItem& item)
{
    long category_def_index = CTCInventoryItem::GetInventoryCategory(item.ItemType);
    std::vector<CCategory>::iterator categories_end = Categories.end();
    std::vector<CCategory>::iterator category_it = std::find_if(Categories.begin(), categories_end, CIsInventoryCategory(category_def_index));
    if (category_it != categories_end && category_it->AddInventoryItem(item)) {
        OnItemAdded(item.ItemType, true, true);
        return true;
    }
    return false;
}
}
