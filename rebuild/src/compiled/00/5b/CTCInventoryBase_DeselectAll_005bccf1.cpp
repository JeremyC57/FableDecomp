// CTCInventoryBase::DeselectAll() -- clears the selection of every category (selected slot -1, highlighted slot 0)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCCF1, length 60 bytes.
// Callees: none (the per-category reset is inline).
// Callers: 0x005B49B9.
// Pragma: optimize("s", on) -- size TU (`push 2Ch ; pop edi` divisor, `or [eax+20h],-1` / `and [eax+24h],0` stores).
// Layout: CTCInventoryBase has no engine header; Ego_r.pdb with the shipped 12-byte vectors gives Categories
//         (vector<CCategory>, 0x2C-byte elements) at +0x20 (PDB +0x24). Retail CCategory: SelectedItemIndex +0x20,
//         HighlightedItemIndex +0x24 (the landed CCategory sources' view).
// Shape: an inline CCategory::Deselect() on `Categories[i]`; the stores may alias the vector, so `first` and
//        size() are re-read every iteration and `this` stays in ecx.

struct CCategory {
    unsigned char PAssociatedThing[0x8];          // +0x00
    unsigned char ItemsInCategory[0x0C];          // +0x08 vector<CItem>
    void*         PDef;                           // +0x14
    void*         PInventory;                     // +0x18
    long          GlobalDefIndex;                 // +0x1C
    long          SelectedItemIndex;              // +0x20
    long          HighlightedItemIndex;           // +0x24
    bool          Selected;                       // +0x28
    bool          DisplayItems;                   // +0x29
    void Deselect()
    {
        SelectedItemIndex = -1;
        HighlightedItemIndex = 0;
    }
};

// Retail std::vector<CCategory> (12 bytes).
struct CCategoryVector {
    CCategory* first;                             // +0x00
    CCategory* last;                              // +0x04
    CCategory* end_of_storage;                    // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    CCategory& operator[](unsigned long n) { return first[n]; }
};

struct CTCInventoryBase_Retail {
    unsigned char   _pad_0x00[0x20];
    CCategoryVector Categories;                   // +0x20 (PDB +0x24)
    void DeselectAll();
};

void CTCInventoryBase_Retail::DeselectAll()
{
    for (unsigned long i = 0; i < Categories.size(); ++i)
        Categories[i].Deselect();
}