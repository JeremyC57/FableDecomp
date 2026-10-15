// CTCInventoryBase::OnKill() -- marks the inventory as dying and passes the kill on to every category (virtual, NInventory::CTCInventoryBase vtable 0x01253DBC slot 14)
// Leaf: ?OnKill@CTCInventoryBase_Retail@@QAEXXZ
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC9C8, length 61 bytes.
// Callees: CCategory::OnKill() @ 0x005BC754 (same TU; its real body is reproduced below and grades MATCH from this
//          file as well).
// Label: the manifest's "CTCInventoryBase::SetQuestPersistentFlagForItems" is propagated; vtable slot 14 is OnKill
//        across the CTCBase family (CTCGraphicAppearance, CTCVillage, CTCCarrying, ... rtti-vtables.tsv), and the
//        byte store is Dying (Ego_r.pdb +0x90, retail +0x88 after two 12-byte vectors).
// Pragma: optimize("s", on) -- size TU (`push 2Ch ; pop esi` divisor).
// Shape: the category count is computed ONCE and `this` is dropped (ecx reused for the element): VC7.1 knows the
//        callee, defined EARLIER IN THE SAME TU, writes no memory, so Categories cannot change across the calls.
//        With CCategory::OnKill only declared the loop re-reads Categories each iteration and keeps `this`.
// Layout: CTCInventoryBase has no engine header; Ego_r.pdb with the shipped 12-byte vectors gives Components
//         +0x0C, Categories +0x20 (vector<CCategory>, 0x2C-byte elements), Dying +0x88.
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte vector<CItem>; the
// shipped vector is {first, last, end} (12 bytes) -- the landed CCategory sources (0x005BC7A6, 0x005BC8AC, ...)
// use the same local view. CItem is 0x14 bytes in the shipped build.

struct CItem {
    unsigned char _body[0x14];
    void OnKill() { }                             // inline, empty in the shipped build
};

// Retail std::vector<CItem> (12 bytes). size() is the signed element difference (`sub ; cdq ; idiv 14h`)
// converted to the unsigned size_type; elements are reached through `first`.
struct CItemVector {
    CItem* first;                                 // +0x00
    CItem* last;                                  // +0x04
    CItem* end_of_storage;                        // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem& operator[](unsigned long n) { return first[n]; }
};

struct CCategory {
    unsigned char PAssociatedThing[0x8];          // +0x00 CIntelligentPointer<CThing>
    CItemVector   ItemsInCategory;                // +0x08
    unsigned char _rest_0x14[0x2C - 0x14];        // PDef, PInventory, indices, flags
    void OnKill();
};

void CCategory::OnKill()
{
    for (unsigned long i = 0; i < ItemsInCategory.size(); ++i)
        ItemsInCategory[i].OnKill();
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
    unsigned char   _pad_0x2c[0x88 - 0x2C];
    bool            Dying;                        // +0x88 (PDB +0x90)
    void OnKill();
};

void CTCInventoryBase_Retail::OnKill()
{
    Dying = true;
    for (unsigned long i = 0; i < Categories.size(); ++i)
        Categories[i].OnKill();
}