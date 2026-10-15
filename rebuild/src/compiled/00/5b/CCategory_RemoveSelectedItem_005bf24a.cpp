// CCategory::RemoveSelectedItem() -- take one item out of the selected slot; an emptied slot clears the selection (selection -1, highlight 0); unless the category selects empty slots, a highlight left on an empty slot moves to the next occupied one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF24A, length 124 bytes; a frame-pointer size TU (`leave`).
// Callees: NInventory::CItem::RemoveItem(long) @ 0x005BF00B, CCategory::GetNextOccupiedSlot @ 0x005BCF61,
// CInventoryCategoryDef virtual slot 1 (+0x04, destroy).
// Shape: `optimize("y", off)` + `("s", on)`; the `!SelectEmptySlots && slot empty` condition is evaluated while the
// scoped CDefPointer reference is alive, so its result is parked in the frame byte [ebp-1] across the release (the
// zeroed dword at [ebp-8] is the same frame artefact and reproduces from this source).
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte vector<CItem>; the shipped
// vector is {first, last, end} (12 bytes), so every member after ItemsInCategory sits 4 bytes lower than the header
// says (the landed CCategory sources 0x005BD1CF / 0x005BC8EB document the same): +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex, +0x24 HighlightedItemIndex,
// +0x28 Selected, +0x29 DisplayItems. The local CCategory_Retail view carries the PDB names at the retail offsets.
#include "engine/CItem.h"

struct CInventoryCategoryDef_Retail {         // the def behind the retail CDefPointer (count at +4, slot 1 destroys)
    virtual void v00();
    virtual void Destroy();                   // slot 1 (+0x04)
    long          m_refCount;                 // +0x04
    unsigned char _pad_0x08[0x38];            // +0x08
    long          NumberOfSlots;              // +0x40
    bool          DrawItemSlots;              // +0x44
    bool          SelectEmptySlots;           // +0x45
    bool          WrapHighlightCursor;        // +0x46
    unsigned char _pad_0x47[0x1];             // +0x47
    unsigned long CategoryName;               // +0x48
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CDefRef {                              // scoped reference returned by CDefPointer::operator->
    CInventoryCategoryDef_Retail* m_p;
    CDefRef(CInventoryCategoryDef_Retail* p) : m_p(p) { if (m_p) m_p->AddRef(); }
    ~CDefRef() { m_p->Release(); }
    CInventoryCategoryDef_Retail* operator->() const { return m_p; }
};
struct CDefPointer {
    CInventoryCategoryDef_Retail* m_pObject;  // Object
    CDefRef operator->() const { return CDefRef(m_pObject); }
};
struct CItemVector {                          // retail std::vector<NInventory::CItem>, 12 bytes
    CItem* first;                             // +0x00
    CItem* last;                              // +0x04
    CItem* end_of_storage;                    // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    CItem& operator[](unsigned long n) { return first[n]; }
};

struct CItem_Methods : CItem {
    void RemoveItem(long count);              // 0x005BF00B
};
struct CItemMethodsVector {                   // retail std::vector<NInventory::CItem>, 12 bytes
    CItem_Methods* first;                     // +0x00
    CItem_Methods* last;                      // +0x04
    CItem_Methods* end_of_storage;            // +0x08
    CItem_Methods* begin() const { return first; }
    CItem_Methods& operator[](unsigned long n) { return *(begin() + n); }   // STLport form
};

struct CCategory_Retail {
    unsigned char      PAssociatedThing[0x8]; // +0x00 CIntelligentPointer<CThing>
    CItemMethodsVector ItemsInCategory;       // +0x08
    CDefPointer        PDef;                  // +0x14
    void*              PInventory;            // +0x18
    long               GlobalDefIndex;        // +0x1c
    long               SelectedItemIndex;     // +0x20
    long               HighlightedItemIndex;  // +0x24
    long GetNextOccupiedSlot(long index);     // 0x005BCF61
    void RemoveSelectedItem();
};

void CCategory_Retail::RemoveSelectedItem()
{
    ItemsInCategory[SelectedItemIndex].RemoveItem(1);
    if (ItemsInCategory[SelectedItemIndex].NumberOfItems == 0) {
        SelectedItemIndex = -1;
        HighlightedItemIndex = 0;
    }
    if (!PDef->SelectEmptySlots && ItemsInCategory[HighlightedItemIndex].NumberOfItems == 0)
        HighlightedItemIndex = GetNextOccupiedSlot(HighlightedItemIndex);
}