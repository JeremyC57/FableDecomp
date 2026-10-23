// CTCInventoryBase::RemoveItemFromInventory(long itemType, long numberOfItems) -- removes `numberOfItems` items of `itemType` from the category that holds that item type
// Leaf: RemoveItemFromInventory@CTCInventoryBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF3C6, length 43 bytes; size TU. Virtual slot 79 (+0x13C) of the inventory
// screens (CTCInventoryBase vtable 0x01253DBC + 0x13C = 0x01253EF8 holds it); the catalog label "GetItemDescription" is propagated.
// Callees: GetInventoryCategory(long) @ 0x005D97B0 (`__fastcall`), the SGI find_if over Categories @ 0x005C08D3 (`__fastcall`
// /Gr template, predicate modelled as its single long), CCategory::RemoveItem(long, long) @ 0x005BF221.
// Retail CTCInventoryBase: no engine header; Categories at +0x20.
// Shape: RemoveItem's two pass-through pushes sit between the GetInventoryCategory call and the find call, so the
// category def index is a named local computed first and the find result feeds the member call directly.
#pragma optimize("s", on)
long __fastcall GetInventoryCategory(long itemType);            // 0x005D97B0
struct CCategory_Retail {
    unsigned char _body[0x2C];
    void RemoveItem(long itemType, long numberOfItems);          // 0x005BF221
};
CCategory_Retail* __fastcall FindCategoryWithDefIndex_005c08d3(CCategory_Retail* first, CCategory_Retail* last, long categoryDefIndex);   // 0x005C08D3
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                                  // +0x20
    void RemoveItemFromInventory(long itemType, long numberOfItems);
};
void CTCInventoryBase_Retail::RemoveItemFromInventory(long itemType, long numberOfItems)
{
    long categoryDefIndex = GetInventoryCategory(itemType);
    FindCategoryWithDefIndex_005c08d3(Categories.first, Categories.last, categoryDefIndex)->RemoveItem(itemType, numberOfItems);
}