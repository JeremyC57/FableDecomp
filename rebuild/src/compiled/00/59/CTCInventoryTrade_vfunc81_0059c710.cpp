// CTCInventoryTrade::vfunc_81(long, long, bool, long, long, long) -- slot 81: adds `count` items of a type to the category of the current trade type with the given silent flag and price; true on success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C710, 55 bytes. rtti-vtables: CTCInventoryTrade 0x01253034 slot 81 (`ret 18h`).
// Callees: CTCInventoryTrade::GetCategoryIndexForTradeType @ 0x0059B6F4, CTCInventoryBase::GetCategoryWithIndex(long)
// @ 0x005BCE3C, NInventory::CCategory::AddItem(long, bool, long, long) @ 0x005BF060 (called with arguments 1, 3, 5, 2).
// Arguments 4 and 6 are not used by this override; the real method name is unknown.
// Shape: `mov edx,ecx ; call GetCategoryIndexForTradeType ; push eax ; mov ecx,edx` -- `this` survives the first call in
// EDX, which only works because VC7.1 knows the registers a function compiled EARLIER IN THE SAME TRANSLATION UNIT
// clobbers (0x0059B6F4 touches only eax/ecx). The callee is therefore defined above the forwarder, its body
// reconstructed from 0x0059B6F4 (a two-entry {category identifier, category index} table scan at 0x0137CCDC, -1 if absent) and
// kept out of line with `#pragma auto_inline(off)`; declared-only, the call clobbers edx and VC uses `push esi`.
// Size TU (`#pragma optimize("s", on)`, the trade module). +0x170 is CTCInventoryTrade::CategoryIdent (PDB +0x19C,
// EInventoryCategoryIdentifier): engine/CTCInventoryTrade.h sits 0x2C bytes above retail at this point (smaller
// retail base, vector, list and maps), so the header is not included and a local model carries the PDB name.
// `if (category && AddItem(...)) return true; return false;` gives the retail `test al,al ; je ; mov al,1 ; jmp` exits.
// The local view is named CTCInventoryTrade_Retail so it does not shadow the PDB header engine/CTCInventoryTrade.h.
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
struct CTCInventoryTrade_Retail : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x170 - 0x130];
    long CategoryIdent;                             // +0x170
    long GetCategoryIndexForTradeType();
    bool vfunc_81(long itemType, long count, bool silent, long unused0, long price, long unused1);
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

bool CTCInventoryTrade_Retail::vfunc_81(long itemType, long count, bool silent, long unused0, long price, long unused1)
{
    NInventory::CCategory* category = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    if (category && category->AddItem(itemType, silent, price, count))
        return true;
    return false;
}