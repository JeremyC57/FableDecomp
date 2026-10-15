// CTCInventoryBase::InitialiseInventoryWithItems(const std::vector<NInventory::CItem>& items) -- adds every saved item stack in `items` to the inventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF8CB, length 79 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on)
// (`this` and the index spilled to the frame, `push 14h ; pop ebx` divisor).
// Callees: CTCInventoryBase::AddItem(const CItem&) @ 0x005BF4A3 (manifest "Inventory_AddItem").
// Retail NInventory::CItem is 0x14 bytes; the retail std::vector is {first, last, end} (12 bytes). size() is the signed
// element difference converted to unsigned (`jb`/`jbe` compares), re-read after every call.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CItem_Retail { unsigned char _body[0x14]; };
struct CItemVector {
    CItem_Retail* first;
    CItem_Retail* last;
    CItem_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    const CItem_Retail& operator[](unsigned long n) const { return first[n]; }
};
struct CTCInventoryBase_Retail {
    void AddItem(const CItem_Retail& item);                      // 0x005BF4A3
    void InitialiseInventoryWithItems(const CItemVector& items);
};
void CTCInventoryBase_Retail::InitialiseInventoryWithItems(const CItemVector& items)
{
    for (unsigned long i = 0; i < items.size(); ++i)
        AddItem(items[i]);
}
