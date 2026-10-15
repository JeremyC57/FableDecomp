// CCategory::OnKill() -- per-item kill notification over the category's item stacks; the per-item hook is inline and empty in the shipped build, so only the counting loop remains
// Leaf: ?OnKill@CCategory@@QAEXXZ
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC754, length 39 bytes.
// Callers: CTCInventoryBase::OnKill @ 0x005BC9C8 (once per category).
// Label: the manifest's "CTCInventoryBase::SetQuestPersistentFlagForItems" is propagated; the body works on a
//        CCategory (item vector at +0x08, 0x14-byte elements) and takes no argument (`ret`).
// Pragma: optimize("s", on) -- size TU (`push 14h ; pop edi` divisor).
// Shape: VC7.1 keeps an empty loop: the entry test computes size(), the loop-invariant size() is hoisted into
//        the preheader (computed a second time, with `ecx` reused for the divisor) and the body is `inc ; cmp ; jb`.
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
