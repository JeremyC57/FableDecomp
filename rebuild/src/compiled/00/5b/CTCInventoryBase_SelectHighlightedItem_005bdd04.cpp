// CTCInventoryBase::SelectHighlightedItem(bool& selected) -- clears `selected`, then lets the selected category select its highlighted item when SelectedCategoryIndex is a valid index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDD04, length 53 bytes; size TU (`push 2Ch ; pop ebx` divisor inside a push/pop ebx pair).
// Callees: CCategory::SelectHighlightedItem(bool&) @ 0x005BD9B2.
// Retail CTCInventoryBase: no engine header; Categories at +0x20 (0x2C-byte elements), SelectedCategoryIndex at +0x70.
// Shape: the signed `>= 0` test (`jl`) then the unsigned size compare (`jae`) against the re-computed element count
// is `idx >= 0 && (unsigned long)idx < size()`; the reference argument stays in edi for the store and the forward.
#pragma optimize("s", on)
struct CCategory_Retail {
    unsigned char _body[0x2C];
    void SelectHighlightedItem(bool& selected);  // 0x005BD9B2
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory_Retail& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                  // +0x20
    unsigned char   _pad_0x2c[0x44];
    long            SelectedCategoryIndex;       // +0x70
    void SelectHighlightedItem(bool& selected);
};
void CTCInventoryBase_Retail::SelectHighlightedItem(bool& selected)
{
    selected = false;
    if (SelectedCategoryIndex >= 0 && (unsigned long)SelectedCategoryIndex < Categories.size())
        Categories[SelectedCategoryIndex].SelectHighlightedItem(selected);
}