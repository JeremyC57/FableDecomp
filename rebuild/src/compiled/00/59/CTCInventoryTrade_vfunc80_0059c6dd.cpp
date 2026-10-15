// CTCInventoryTrade::vfunc_80(CThing*, bool, long, long, long) -- slot 80: adds a thing to the category of the current trade type with the given silent flag and price; true on success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059C6DD, 51 bytes. rtti-vtables: CTCInventoryTrade 0x01253034 slot 80 (`ret 14h`).
// Callees: CTCInventoryTrade::GetCategoryIndexForTradeType @ 0x0059B6F4, CTCInventoryBase::GetCategoryWithIndex(long)
// @ 0x005BCE3C, NInventory::CCategory::AddItem(CThing*, bool, long) @ 0x005BF5A4. Arguments 3 and 5 are not used by this
// override (named unused0/unused1); the real method name is unknown.
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
struct CThing;
namespace NInventory {
struct CCategory {
    bool AddItem(CThing* thing, bool silent, long price);               // 0x005BF5A4
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
    bool vfunc_80(CThing* thing, bool silent, long unused0, long price, long unused1);
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

bool CTCInventoryTrade_Retail::vfunc_80(CThing* thing, bool silent, long unused0, long price, long unused1)
{
    NInventory::CCategory* category = GetCategoryWithIndex(GetCategoryIndexForTradeType());
    if (category && category->AddItem(thing, silent, price))
        return true;
    return false;
}