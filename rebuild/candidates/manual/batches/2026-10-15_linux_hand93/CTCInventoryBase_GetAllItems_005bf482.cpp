// CTCInventoryBase::GetAllItems(std::vector<NInventory::CItem>& items) -- appends every category's items to `items`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF482, length 33 bytes; size TU (rotated loop entered by `jmp` to the test).
// Callees: CCategory::GetAllItemsInCategory(vector<CItem>&) @ 0x005BF2C6.
// Retail CTCInventoryBase: no engine header; Categories {first, last, end} at +0x20 (0x2C-byte CCategory elements).
// Shape: a pointer walk `for (it = first; it != last; ++it)` with `last` re-read every iteration (the call could grow the vector).
#pragma optimize("s", on)
struct CItemVector;
struct CCategory_Retail {
    unsigned char _body[0x2C];
    void GetAllItemsInCategory(CItemVector& items);              // 0x005BF2C6
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                                  // +0x20
    void GetAllItems(CItemVector& items);
};
void CTCInventoryBase_Retail::GetAllItems(CItemVector& items)
{
    for (CCategory_Retail* it = Categories.first; it != Categories.last; ++it)
        it->GetAllItemsInCategory(items);
}
