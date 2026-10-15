// CCategory::GetPreviousOccupiedSlot(long index) -- the last occupied inventory slot before `index`; when the category wraps its highlight cursor, continue from the last slot down to `index`; `index` itself when nothing is found
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BCFE2, length 114 bytes (ret 4); size-optimised TU (`push 14h ; pop
// ecx` divisor). Called by CCategory::HighlightPreviousItem (0x005BD0F7).
// Callees: CInventoryCategoryDef virtual slot 1 (+0x04, destroy) when the def's count drops to zero.
// Shape: both scans are SIGNED downward loops with the slot pointer strength-reduced (`sub ecx,14h`); the wrapped scan
// starts at `(long)size() - 1`; WrapHighlightCursor (+0x46) is read through the scoped CDefPointer reference.
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

struct CCategory_Retail {
    unsigned char PAssociatedThing[0x8];      // +0x00 CIntelligentPointer<CThing>
    CItemVector   ItemsInCategory;            // +0x08
    CDefPointer   PDef;                       // +0x14
    long GetPreviousOccupiedSlot(long index);
};

long CCategory_Retail::GetPreviousOccupiedSlot(long index)
{
    for (long i = index - 1; i >= 0; --i) {
        if (ItemsInCategory[i].NumberOfItems != 0)
            return i;
    }
    if (PDef->WrapHighlightCursor) {
        for (long j = (long)ItemsInCategory.size() - 1; j >= index; --j) {
            if (ItemsInCategory[j].NumberOfItems != 0)
                return j;
        }
    }
    return index;
}