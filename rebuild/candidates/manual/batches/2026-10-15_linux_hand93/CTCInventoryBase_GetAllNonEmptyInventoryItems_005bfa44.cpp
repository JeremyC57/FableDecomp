// CTCInventoryBase::GetAllNonEmptyInventoryItems(std::vector<NInventory::CItem>& items, bool includeAll) -- appends every category's non-empty stacks (all of them, or only the ones its item filter accepts) to `items`
// Leaf: GetAllNonEmptyInventoryItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFA44, length 37 bytes; size TU. The catalog label "GetAllInventoryItemsOfType"
// is propagated from the twin 0x005BFA69; this body forwards to CCategory::GetAllNonEmptyInventoryItemsInCategory (vector&, bool)
// and ConfiscateAllItems (0x005BFA8E) calls it as (&items, true).
// Callees: CCategory::GetAllNonEmptyInventoryItemsInCategory @ 0x005BF97D (skips empty stacks; with includeAll false it also asks
// CItem @ 0x005BC729 whether the stack qualifies).
// Retail CTCInventoryBase: no engine header; Categories at +0x20.
// Shape: the two pass-through pushes (`push dword ptr [esp+10h]` twice) keep the source argument order.
#pragma optimize("s", on)
struct CItemVector;
struct CCategory_Retail {
    unsigned char _body[0x2C];
    void GetAllNonEmptyInventoryItemsInCategory(CItemVector& items, bool includeAll);   // 0x005BF97D
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                                  // +0x20
    void GetAllNonEmptyInventoryItems(CItemVector& items, bool includeAll);
};
void CTCInventoryBase_Retail::GetAllNonEmptyInventoryItems(CItemVector& items, bool includeAll)
{
    for (CCategory_Retail* it = Categories.first; it != Categories.last; ++it)
        it->GetAllNonEmptyInventoryItemsInCategory(items, includeAll);
}
