// NInventory::CTCInventoryBase::ReturnAllConfiscatedItems() -- gives back every confiscated item stack, then empties ConfiscatedItems
// Leaf: ?ReturnAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF91A, length 79 bytes; size TU (`push 14h ; pop ecx` divisor, no frame:
// ebp is the stride register). Virtual slot 73 of the inventory screens (CTCInventoryClothing vtable 0x0124356C).
// Callees: CTCInventoryBase::AddItem(const CItem&) @ 0x005BF4A3, std::vector<CItem>::erase(first, last) @ 0x005C1ECC.
// Shape: the trailing erase reads both bounds through the vector pointer (`push [esi+4] ; mov ecx,esi ; push [esi]`), i.e. the
// erase(begin(), end()) sits in an inline member of the vector (`clear()`); written at the call site it re-addresses
// `[edi+0B0h]` from `this`.
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 73);
// Ego_r.exe ?ReturnAllConfiscatedItems@CTCInventoryBase@NInventory@@UAEXXZ 0x006229CE.
// Names (NF95, 2026-11-19, queue q11240): moved off the row-local CTCInventoryBase / CItem_Retail / CItemVector models onto
//   engine/NInventory_CTCInventoryBase.h (PASS22, ConfiscatedItems +0xAC via FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED) and
//   stlport/fable_containers.h (range erase out of line: FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE), so the calls bind Ego_r.pdb /
//   FableWin.pdb ?AddInventoryItem@CTCInventoryBase@NInventory@@AAE_NABVCItem@2@@Z (0x005BF4A3) and the STLport
//   vector<NInventory::CItem>::erase(first, last) spelling the landed 0x005C2B8B binds (0x005C1ECC).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ORIGINAL_COMPLETE_METHODS_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::ReturnAllConfiscatedItems()
{
    for (unsigned long i = 0; i < ConfiscatedItems.size(); ++i)
        AddInventoryItem(ConfiscatedItems[i]);
    ConfiscatedItems.clear();
}
}
