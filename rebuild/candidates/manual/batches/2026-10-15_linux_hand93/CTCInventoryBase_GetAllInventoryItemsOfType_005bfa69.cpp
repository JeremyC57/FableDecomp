// CTCInventoryBase::GetAllInventoryItemsOfType(long itemType, std::vector<NInventory::CItem>& items) -- appends every category's stacks of `itemType` to `items`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFA69, length 37 bytes; size TU.
// Callees: CCategory::GetAllInventoryItemsOfTypeInCategory(long, vector<CItem>&) @ 0x005BF9B5.
// Retail CTCInventoryBase: no engine header; Categories at +0x20.
// Callers: ConfiscateAllItemsOfType 0x005BFAC6 and GetBestFishWeight 0x005BFC90 pass (type, &items).
#pragma optimize("s", on)
struct CItemVector;
struct CCategory_Retail {
    unsigned char _body[0x2C];
    void GetAllInventoryItemsOfTypeInCategory(long itemType, CItemVector& items);   // 0x005BF9B5
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                                  // +0x20
    void GetAllInventoryItemsOfType(long itemType, CItemVector& items);
};
void CTCInventoryBase_Retail::GetAllInventoryItemsOfType(long itemType, CItemVector& items)
{
    for (CCategory_Retail* it = Categories.first; it != Categories.last; ++it)
        it->GetAllInventoryItemsOfTypeInCategory(itemType, items);
}
