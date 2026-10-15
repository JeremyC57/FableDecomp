// CTCInventoryBase::HighlightNextItem() -- while the inventory is in item-selection state (1) and no category slide is running, plays the highlight sound and moves the selected category's highlight to the next item
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDCB4, length 40 bytes; size TU (the landed CTCInventoryBase module is "s").
// Callees: CTCInventoryBase sound helper @ 0x005BD33C (member, no arguments; manifest "PlaySoundForMeshAnimation"),
// CCategory::HighlightNextItem @ 0x005BD054 (tail jump on &Categories[SelectedCategoryIndex]).
// Retail CTCInventoryBase: no engine header; PDB layout minus 4 bytes per preceding std::vector:
// InventorySelectionState +0x1C, Categories +0x20 (0x2C-byte elements), SelectedCategoryIndex +0x70, CategoryMoving +0x89.
#pragma optimize("s", on)
struct CCategory_Retail {
    unsigned char _body[0x2C];
    void HighlightNextItem();                    // 0x005BD054
    void HighlightPreviousItem();                // 0x005BD0F7
};
struct CCategoryVector {
    CCategory_Retail* first;
    CCategory_Retail* last;
    CCategory_Retail* end_of_storage;
    CCategory_Retail& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x1C];
    long            InventorySelectionState;     // +0x1C
    CCategoryVector Categories;                  // +0x20
    unsigned char   _pad_0x2c[0x44];
    long            SelectedCategoryIndex;       // +0x70
    unsigned char   _pad_0x74[0x15];
    bool            CategoryMoving;              // +0x89
    void PlayHighlightSound();                   // 0x005BD33C
    void HighlightNextItem();
};
void CTCInventoryBase_Retail::HighlightNextItem()
{
    if (InventorySelectionState == 1 && !CategoryMoving) {
        PlayHighlightSound();
        Categories[SelectedCategoryIndex].HighlightNextItem();
    }
}
