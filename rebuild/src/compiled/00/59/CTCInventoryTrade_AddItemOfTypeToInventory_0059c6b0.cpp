// CTCInventoryTrade::AddItemOfTypeToInventory(long, long) -- adds `count` items of a type to the category of the current trade type (not silently, price -1); false when there is no such category
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C6B0, 45 bytes. Callees: CTCInventoryTrade::GetCategoryIndexForTradeType
// @ 0x0059B6F4, CTCInventoryBase::GetCategoryWithIndex(long) @ 0x005BCE3C, NInventory::CCategory::AddItem(long, bool, long,
// long) @ 0x005BF060 (bool in al, returned as is: `jmp` over the `xor al,al` of the null path).
// Shape: `mov edx,ecx ; call GetCategoryIndexForTradeType ; push eax ; mov ecx,edx` -- `this` survives the first call in
// EDX, which only works because VC7.1 knows the registers a function compiled EARLIER IN THE SAME TRANSLATION UNIT
// clobbers (0x0059B6F4 touches only eax/ecx). The callee is therefore defined above the forwarder, its body
// reconstructed from 0x0059B6F4 (a two-entry {category identifier, category index} table scan at 0x0137CCDC, -1 if absent) and
// kept out of line with `#pragma auto_inline(off)`; declared-only, the call clobbers edx and VC uses `push esi`.
// Size TU (`#pragma optimize("s", on)`, the trade module). +0x170 is CTCInventoryTrade::CategoryIdent (PDB +0x19C,
// EInventoryCategoryIdentifier): engine/CTCInventoryTrade.h sits 0x2C bytes above retail at this point (smaller
// retail base, vector, list and maps), so the header is not included and a local model carries the PDB name.
// The by-value arguments are re-pushed from their slots (`push dword ptr [esp+8]` ... `push dword ptr [esp+10h]`).
// The local view is named CTCInventoryTrade so it does not shadow the PDB header engine/CTCInventoryTrade.h.
#pragma optimize("s", on)
class CThing;
namespace NInventory {
class CCategory { public:
    bool AddItem(long itemType, bool silent, long price, long count);   // 0x005BF060
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
class CTCInventoryTrade : CTCInventoryBase_Retail { public:
    unsigned char _pad_0x130[0x170 - 0x130];
    long CategoryIdent;                             // +0x170
    bool AddItemOfTypeToInventory(long itemType, long count);
public:
    long GetCategoryIndexForTradeType();   // 0x0059B6F4
};
#pragma auto_inline(off)
long CTCInventoryTrade::GetCategoryIndexForTradeType()
{
    for (long i = 0; i < 2; ++i)
        if (g_categoryIdentIndices[i].CategoryIdent == CategoryIdent)
            return g_categoryIdentIndices[i].CategoryIndex;
    return -1;
}
#pragma auto_inline(on)

bool CTCInventoryTrade::AddItemOfTypeToInventory(long itemType, long count)
{
    NInventory::CCategory* category = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    if (category)
        return category->AddItem(itemType, false, -1, count);
    return false;
}