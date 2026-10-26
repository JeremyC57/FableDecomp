// CTCInventoryTrade::vfunc_79(long, long) -- slot 79: removes `count` items of a type from the category of the current trade type, if there is one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C747, 29 bytes. rtti-vtables: CTCInventoryTrade 0x01253034 slot 79.
// Callees: CTCInventoryTrade::GetCategoryIndexForTradeType @ 0x0059B6F4, CTCInventoryBase::GetCategoryWithIndex(long)
// @ 0x005BCE3C, NInventory::CCategory::RemoveItem(long, long) @ 0x005BF221 (tail jump reusing the two argument slots).
// The real method name is unknown (catalog label kept).
// Shape: `mov edx,ecx ; call GetCategoryIndexForTradeType ; push eax ; mov ecx,edx` -- `this` survives the first call in
// EDX, which only works because VC7.1 knows the registers a function compiled EARLIER IN THE SAME TRANSLATION UNIT
// clobbers (0x0059B6F4 touches only eax/ecx). The callee is therefore defined above the forwarder, its body
// reconstructed from 0x0059B6F4 (a two-entry {category identifier, category index} table scan at 0x0137CCDC, -1 if absent) and
// kept out of line with `#pragma auto_inline(off)`; declared-only, the call clobbers edx and VC uses `push esi`.
// Size TU (`#pragma optimize("s", on)`, the trade module). +0x170 is CTCInventoryTrade::CategoryIdent (PDB +0x19C,
// EInventoryCategoryIdentifier): engine/CTCInventoryTrade.h sits 0x2C bytes above retail at this point (smaller
// retail base, vector, list and maps), so the header is not included and a local model carries the PDB name.
// The local view is named CTCInventoryTrade_Retail so it does not shadow the PDB header engine/CTCInventoryTrade.h.
#pragma optimize("s", on)
namespace NInventory {
class CCategory { public:
    void RemoveItem(long itemType, long count);     // 0x005BF221
};
}
struct SCategoryIdentIndex {
    long CategoryIdent;
    long CategoryIndex;
};
extern SCategoryIdentIndex g_categoryIdentIndices[2];   // 0x0137CCDC
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    NInventory::CCategory* GetCategoryWithIndex(long index);   // 0x005BCE3C
};
struct CTCInventoryTrade_Retail : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x170 - 0x130];
    long CategoryIdent;                             // +0x170
    long GetCategoryIndexForTradeType();
    void vfunc_79(long itemType, long count);
};
#pragma auto_inline(off)
long CTCInventoryTrade_Retail::GetCategoryIndexForTradeType()
{
    for (long i = 0; i < 2; ++i)
        if (g_categoryIdentIndices[i].CategoryIdent == CategoryIdent)
            return g_categoryIdentIndices[i].CategoryIndex;
    return -1;
}
#pragma auto_inline(on)
void CTCInventoryTrade_Retail::vfunc_79(long itemType, long count)
{
    NInventory::CCategory* category = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    if (category)
        category->RemoveItem(itemType, count);
}