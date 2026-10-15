// CTCInventoryBase::ReturnAllConfiscatedItems() -- gives back every confiscated item stack, then empties ConfiscatedItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF91A, length 79 bytes; size TU (`push 14h ; pop ecx` divisor, no frame:
// ebp is the stride register). Virtual slot 73 of the inventory screens (CTCInventoryClothing vtable 0x0124356C).
// Callees: CTCInventoryBase::AddItem(const CItem&) @ 0x005BF4A3, std::vector<CItem>::erase(first, last) @ 0x005C1ECC.
// Retail CTCInventoryBase: no engine header; ConfiscatedItems (std::vector<NInventory::CItem>, 0x14-byte elements) at +0xAC
// (PDB +0xBC minus four 4-byte-narrower vectors before it).
// Shape: the trailing erase reads both bounds through the vector pointer (`push [esi+4] ; mov ecx,esi ; push [esi]`), i.e. the
// erase(begin(), end()) sits in an inline member of the vector (`clear()`); written at the call site it re-addresses
// `[edi+0B0h]` from `this`.
#pragma optimize("s", on)
struct CItem_Retail { unsigned char _body[0x14]; };
struct CItemVector {
    CItem_Retail* first;
    CItem_Retail* last;
    CItem_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem_Retail& operator[](unsigned long n) { return first[n]; }
    CItem_Retail* begin() { return first; }
    CItem_Retail* end() { return last; }
    CItem_Retail* erase(CItem_Retail* from, CItem_Retail* to);   // 0x005C1ECC
    void clear() { erase(begin(), end()); }
};
struct CTCInventoryBase_Retail {
    unsigned char _pad_0x00[0xAC];
    CItemVector   ConfiscatedItems;                              // +0xAC
    void AddItem(const CItem_Retail& item);                      // 0x005BF4A3
    void ReturnAllConfiscatedItems();
};
void CTCInventoryBase_Retail::ReturnAllConfiscatedItems()
{
    for (unsigned long i = 0; i < ConfiscatedItems.size(); ++i)
        AddItem(ConfiscatedItems[i]);
    ConfiscatedItems.clear();
}
