// NInventory::CTCInventoryBase::GetAllItems(std::vector<std::pair<long,long> >& items) const -- appends every category's items (type, count) to `items`
// Leaf: ?GetAllItems@CTCInventoryBase@NInventory@@QBEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF482, length 33 bytes; size TU (rotated loop entered by `jmp` to the test).
// Callees: NInventory::CCategory::GetAllItemsInCategory(vector<pair<long,long> >&) const @ 0x005BF2C6.
// Shape: a pointer walk over Categories (+0x20) with `last` re-read every iteration (the call could grow the vector).
// Name fix (agentNF89, q11171): Ego_r.pdb ?GetAllItems@CTCInventoryBase@NInventory@@QBEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@..@std@@@Z (const,
//   the vector of (type, count) pairs); engine/NInventory_CTCInventoryBase.h declares it at this address (BUILDINGTHING) and the landed 0x006276F0 /
//   0x0059D820 bind it. Moved off the row-local CTCInventoryBase_Retail / CCategory_Retail / CItemVector models onto the header's PASS22 chain
//   (Categories as std::vector<NInventory::CCategory>).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#include "engine/NInventory_CTCInventoryBase.h"
namespace NInventory {
void CTCInventoryBase::GetAllItems(std::vector<std::pair<long, long> >& items) const
{
    for (std::vector<CCategory>::const_iterator it = Categories.begin(); it != Categories.end(); ++it)
        it->GetAllItemsInCategory(items);
}
}
