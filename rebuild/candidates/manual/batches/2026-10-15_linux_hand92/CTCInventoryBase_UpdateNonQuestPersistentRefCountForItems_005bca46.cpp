// CTCInventoryBase::UpdateNonQuestPersistentRefCountForItems(bool) -- updates the non-quest-persistent reference counts of the items of every category
// Leaf: ?UpdateNonQuestPersistentRefCountForItems@CTCInventoryBase_Retail
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCA46, length 65 bytes.
// Callees: CCategory::UpdateNonQuestPersistentRefCountForItems(bool) @ 0x005BC7EE (landed), once per category.
// Callers: 0x005BCF53 and 0x005BCF5B.
// Pragma: optimize("s", on) -- size TU (`push 2Ch ; pop ecx` divisor, pass-through `push [esp+10h]`).
// Layout: CTCInventoryBase has no engine header; Ego_r.pdb with the shipped 12-byte vectors gives Categories
//         (vector<CCategory>, 0x2C-byte elements) at +0x20 (PDB +0x24).
// Shape: the callee lives in another TU, so the category count is re-evaluated after every call and `this`
//        stays in esi (compare CTCInventoryBase::OnKill 0x005BC9C8, whose same-TU callee lets VC hoist it).

struct CCategory {
    unsigned char _body[0x2C];
    void UpdateNonQuestPersistentRefCountForItems(bool increment);   // 0x005BC7EE
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
    void UpdateNonQuestPersistentRefCountForItems(bool increment);
};

void CTCInventoryBase_Retail::UpdateNonQuestPersistentRefCountForItems(bool increment)
{
    for (unsigned long i = 0; i < Categories.size(); ++i)
        Categories[i].UpdateNonQuestPersistentRefCountForItems(increment);
}
