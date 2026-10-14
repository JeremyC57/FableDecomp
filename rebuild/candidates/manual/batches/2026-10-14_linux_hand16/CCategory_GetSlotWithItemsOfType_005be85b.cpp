#pragma optimize("y", off)
#pragma optimize("s", on)
// CCategory::GetSlotWithItemsOfType(long) -- finds the slot holding the item type (honouring the def's AllowItemsToFillMoreThanOneSlot) and returns its index, -1 when absent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE85B, length 86 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: find_if<CItem*, SItemMatcher> @ 0x005C1921; CInventoryCategoryDef virtual slot 1 (destroy) from the scoped def reference.
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
// The scoped def reference (AddRef, read the flag byte, Release) is a temporary of the find_if full
// expression: retail releases it (`dec [esi+4]`) right after the call, before comparing the result with
// `last`. The result rides in ebx across the release.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
};
// Retail std::vector<CItem> (12 bytes). size() is the signed element difference (`sub ; cdq ; idiv 14h`)
// converted to the unsigned size_type; elements are reached through `first`.
struct CItemVector {
    CItem_Methods* first;                        // +0x00
    CItem_Methods* last;                         // +0x04
    CItem_Methods* end_of_storage;               // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    bool empty() const { return first == last; }
    CItem_Methods& operator[](unsigned long n) { return first[n]; }
};
// CInventoryCategoryDef through the retail CDefPointer: the pointee is intrusively ref-counted
// (count at +4, virtual slot 1 destroys it). `PDef->` hands out a scoped reference that AddRefs
// (null-checked) and Releases (not null-checked) around the member read -- the retail
// `inc [ecx+4]` / `dec [ecx+4] ; jne ; call [eax+4]` pair.
struct CInventoryCategoryDef_Retail {
    virtual void v00();
    virtual void Destroy();                      // slot 1 (+0x04)
    long          m_refCount;                    // +0x04
    unsigned char _pad_0x08[0x38];               // +0x08
    long          NumberOfSlots;                 // +0x40 (PDB +0x44, retail base is 4 bytes shorter)
    bool          DrawItemSlots;                 // +0x44
    bool          SelectEmptySlots;              // +0x45
    bool          WrapHighlightCursor;           // +0x46
    unsigned char _pad_0x47[0x1];                // +0x47
    unsigned long CategoryName;                  // +0x48
    bool          AllowItemsToFillMoreThanOneSlot; // +0x4C (PDB +0x50)
    unsigned char _pad_0x4d[0x3];                // +0x4D
    long          CategoryIdentifier;            // +0x50 (PDB +0x54)
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};

struct CDefRef {                                 // scoped reference returned by CDefPointer::operator->
    CInventoryCategoryDef_Retail* m_p;
    CDefRef(CInventoryCategoryDef_Retail* p) : m_p(p) { if (m_p) m_p->AddRef(); }
    ~CDefRef() { m_p->Release(); }
    CInventoryCategoryDef_Retail* operator->() const { return m_p; }
};

struct CDefPointer {
    CInventoryCategoryDef_Retail* m_pObject;     // Object
    CDefRef operator->() const { return CDefRef(m_pObject); }
};
// Predicate object for the find_if instantiation: {item type, allow-multiple-slots flag}, an 8-byte POD with
// a user constructor and no copy constructor, so it is pushed dword by dword (the flag byte is homed on the
// stack and pushed as a dword, the type dword is forwarded from a register).
struct SItemMatcher {
    long m_itemType;                             // +0x00
    bool m_allowMultipleSlots;                   // +0x04
    SItemMatcher(long itemType, bool allowMultipleSlots) : m_itemType(itemType), m_allowMultipleSlots(allowMultipleSlots) { }
};
CItem_Methods* __fastcall FindMatchingItem(CItem_Methods* first, CItem_Methods* last, SItemMatcher matcher);   // 0x005C1921

struct CCategory_Retail {
    unsigned char PAssociatedThing[0x8];         // +0x00 CIntelligentPointer<CThing>
    CItemVector   ItemsInCategory;               // +0x08
    CDefPointer   PDef;                          // +0x14
    long GetSlotWithItemsOfType(long itemType);
};

long CCategory_Retail::GetSlotWithItemsOfType(long itemType)
{
    CItem_Methods* found = FindMatchingItem(ItemsInCategory.first, ItemsInCategory.last,
                                            SItemMatcher(itemType, PDef->AllowItemsToFillMoreThanOneSlot));
    if (found != ItemsInCategory.last)
        return (long)(found - ItemsInCategory.first);
    return -1;
}
