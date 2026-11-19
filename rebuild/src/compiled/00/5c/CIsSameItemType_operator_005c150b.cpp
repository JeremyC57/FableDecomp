#pragma optimize("s", on)
// CIsSameItemType::operator()(const NInventory::CItem& item) const -- the item has this predicate's item type and, when the predicate
//   also requires room, its slot is not full.
// Leaf: ??RCIsSameItemType@@QBE_NABVCItem@NInventory@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C150B, 44 bytes, /Os (push [esi], cmp byte [esi+4],0, xor/inc true), ret 4.
// Callees: NInventory::CItem::IsSameItemTypeAsDefIndex @ 0x005BC450, NInventory::CItem::IsSlotFull @ 0x005BDE50. Retail calls IsSlotFull with
//   ecx left over from IsSameItemTypeAsDefIndex: VC7.1 knows that callee leaves ecx alone only from a same-TU, already-compiled definition,
//   so this row carries the owner's body (GOTCHAS nf96a: same-TU strong callee copy).
// Layout: engine/CCategory.h FABLE_RETAIL_CCATEGORY_ITEMQUERIES CIsSameItemType {ItemType +0, OnlyFindNonFullSlots +4} (Ego_r.pdb layout);
//   NInventory::CItem from engine/NInventory_CTCInventoryBase.h (ItemType +0, NumberOfItems +4).
// Name fix (NF100, round 9, q11237): Ego_r.pdb 0x006237C7 + FableWin.pdb 0x02376623 ??RCIsSameItemType@@QBE_NABVCItem@NInventory@@@Z (the
//   CCategory.h declaration). The old row defined ??RCIsSameItemType@@QBE_NPAVCItem@@@Z on a local CIsSameItemType / CItem_Methods view and bound
//   ?IsSameItemTypeAsDefIndex@CItem_Methods@@QAE_NJ@Z / ?IsSlotFull@CItem_Methods@@QAE_NXZ; it now binds ?IsSameItemTypeAsDefIndex@CItem@NInventory@@QBE_NJ@Z
//   (Ego_r.pdb 0x0061F2D6 + FableWin.pdb 0x0235F53C; overlay opt-in FABLE_RETAIL_NINVENTORY_CITEM_ISSAMEITEMTYPEASDEFINDEX) and
//   ?IsSlotFull@CItem@NInventory@@QBE_NXZ (FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CCATEGORY_ITEMQUERIES
#define FABLE_RETAIL_NINVENTORY_CITEM_ISSLOTFULL
#define FABLE_RETAIL_NINVENTORY_CITEM_ISSAMEITEMTYPEASDEFINDEX
#include "engine/NInventory_CTCInventoryBase.h"

// Fidelity: same-TU strong definition of 0x005BC450 (retail TU-mate)
#pragma auto_inline(off)
bool NInventory::CItem::IsSameItemTypeAsDefIndex(long defIndex) const
{
    if (this->NumberOfItems == 0)
        return false;
    return defIndex == this->ItemType;
}
#pragma auto_inline(on)

bool CIsSameItemType::operator()(const NInventory::CItem& item) const
{
    return item.IsSameItemTypeAsDefIndex(ItemType) && (!OnlyFindNonFullSlots || !item.IsSlotFull());
}
