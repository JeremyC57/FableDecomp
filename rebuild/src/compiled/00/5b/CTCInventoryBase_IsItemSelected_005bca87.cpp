// CTCInventoryBase::IsItemSelected(long) -- whether any category currently has the given item type selected
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCA87, length 71 bytes.
// Callees: CCategory::GetSelectedItem() @ 0x005BC99C (landed as "CCategory::GetHighlightedItem": the ItemType of
//          the stack at SelectedItemIndex (+0x20), 0 when none is selected). It belongs to the same TU and its
//          body is reproduced below (byte-identical to the landed 0x005BC99C body).
// Pragma: optimize("s", on) -- size TU (`push 2Ch ; pop ecx` divisor, shared `mov al,1` exit placed last).
// Layout: CTCInventoryBase has no engine header; Ego_r.pdb with the shipped 12-byte vectors gives Categories
//         (vector<CCategory>, 0x2C-byte elements) at +0x20 (PDB +0x24).
// Shape: the selected type is the left operand of `==` (`call ; cmp eax,[esp+10h]`); size() is re-evaluated
//        after every call, but the element pointer is strength-reduced from ONE read of Categories.first
//        (`mov edi,[esi+20h]` before the loop, `add edi,2Ch`): VC7.1 knows the same-TU getter writes no memory.
//        With the getter only declared it re-reads `first` every iteration (73v71).
// Leaf note: the TU also defines CCategory::GetSelectedItem; the line-1 name IsItemSelected is unique.

struct CItem {
    long          ItemType;                       // +0x00
    unsigned char _rest[0x10];
};

struct CCategory {
    unsigned char PAssociatedThing[0x8];          // +0x00
    CItem*        ItemsFirst;                     // +0x08 vector<CItem>::first
    CItem*        ItemsLast;                      // +0x0C
    CItem*        ItemsEnd;                       // +0x10
    unsigned char _pad_0x14[0x0C];
    long          SelectedItemIndex;              // +0x20
    unsigned char _rest_0x24[0x08];
    long GetSelectedItem();                       // 0x005BC99C (defined below: same TU)
};

long CCategory::GetSelectedItem()
{
    long index = SelectedItemIndex;
    if (index >= 0)
        return ItemsFirst[index].ItemType;
    return 0;
}

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
    bool IsItemSelected(long itemType);
};

bool CTCInventoryBase_Retail::IsItemSelected(long itemType)
{
    for (unsigned long i = 0; i < Categories.size(); ++i) {
        if (Categories[i].GetSelectedItem() == itemType)
            return true;
    }
    return false;
}