#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::GetQuickAccessItemDef(unsigned long index) -- inventory-item definition of the item in a combined slot, a null def pointer for an empty slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581DDB, length 88 bytes; size-optimised TU with a frame
// pointer (`optimize("y",off)` + `("s",on)`).
// Callees: GetPDef(long) 0x005D97D0 (__fastcall by-value CDefPointer<const CInventoryItemDef>: result slot in
//          ecx, item type in edx); CTCInventory::GetQuickAccessItem (0x00581E33) is inlined.
// Ego_r PDB: ?GetQuickAccessItemDef@CTCInventory@@QAE?AV?$CDefPointer@$$CBVCInventoryItemDef@@@@K@Z.
// Retail layout: engine/CTCInventory.h is the wider Ego_r.pdb layout (16-byte vectors); the shipped object
// keeps QuickAccessItems / ContextSensitiveItems as 12-byte {first,last,end} vector<long> at +0x148 / +0x154,
// so `this` is modelled locally. A size TU only inlines GetQuickAccessItem when it is `__forceinline`.
// Shape: `ret 8` (hidden result + index). CDefPointer has user constructors, so it returns by hidden pointer
// and GetPDef builds straight into our result slot (`mov ecx,[ebp+8]`); the null branch is the default
// constructor (`and [eax],0`).
struct CInventoryItemDef;

template<class T> struct CDefPointer {
    T* Object;                                       // +0x00
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other);
};

CDefPointer<const CInventoryItemDef> __fastcall GetPDef(long itemType);   // 0x005D97D0

struct CLongVector {
    long* first;                                     // +0x00
    long* last;                                      // +0x04
    long* end_of_storage;                            // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    long& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventory {
    unsigned char _pad_0x000[0x148];
    CLongVector   QuickAccessItems;                  // +0x148 (PDB +0x168)
    CLongVector   ContextSensitiveItems;             // +0x154 (PDB +0x178)
    __forceinline long GetQuickAccessItem(unsigned long index)   // 0x00581E33 (inlined here)
    {
        if (index < ContextSensitiveItems.size())
            return ContextSensitiveItems[index];
        return QuickAccessItems[index - ContextSensitiveItems.size()];
    }
    CDefPointer<const CInventoryItemDef> GetQuickAccessItemDef(unsigned long index);
};

CDefPointer<const CInventoryItemDef> CTCInventory::GetQuickAccessItemDef(unsigned long index)
{
    long item = GetQuickAccessItem(index);
    if (item > 0)
        return GetPDef(item);
    return CDefPointer<const CInventoryItemDef>();
}
