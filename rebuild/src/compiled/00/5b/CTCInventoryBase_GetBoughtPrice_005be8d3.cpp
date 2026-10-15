// CTCInventoryBase::GetBoughtPrice(long itemType) -- finds the category that holds items of `itemType` and returns that category's bought price for it
// Leaf: GetBoughtPrice@CTCInventoryBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE8D3, length 32 bytes; size TU. The catalog label "GetItemDescription" is
// propagated over several CTCInventoryBase category forwarders; this one ends in CCategory::GetBoughtPrice.
// Callees: GetInventoryCategory(long itemDefIndex) @ 0x005D97B0 (`__fastcall`: the item's category def index or -1),
// the SGI find_if over Categories @ 0x005C08C0 (`__fastcall` /Gr template: first in ecx, last in edx, the by-value
// "GlobalDefIndex == x" predicate on the stack; modelled with the predicate's single long), CCategory::GetBoughtPrice @ 0x005BE8B1.
// Retail CTCInventoryBase: no engine header; Categories {first, last, end} at +0x20.
// Shape: the found category goes through a named local so the final member call is a sibling `jmp` that reuses the
// incoming argument slot (the chained `Find(...)->GetBoughtPrice(itemType)` pre-pushes itemType and calls).
#pragma optimize("s", on)
long __fastcall GetInventoryCategory(long itemType);            // 0x005D97B0
struct CCategory_Retail {
    unsigned char _body[0x2C];
    long GetBoughtPrice(long itemType);                          // 0x005BE8B1
};
CCategory_Retail* __fastcall FindCategoryWithDefIndex(CCategory_Retail* first, CCategory_Retail* last, long categoryDefIndex);   // 0x005C08C0
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                                  // +0x20
    long GetBoughtPrice(long itemType);
};
long CTCInventoryBase_Retail::GetBoughtPrice(long itemType)
{
    CCategory_Retail* category = FindCategoryWithDefIndex(Categories.first, Categories.last, GetInventoryCategory(itemType));
    return category->GetBoughtPrice(itemType);
}