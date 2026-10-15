// CCategory::HighlightPreviousItem() -- move the inventory highlight one slot back: categories that select empty slots step to the previous slot (wrapping to the last slot or clamping at 0), others jump to the previous occupied slot; tell the inventory when the highlight changed
// Grade: DIFFER(132v140) -- only the SelectEmptySlots step differs: retail `mov eax,[esi+24h] ; dec eax ; js join ; test eax,eax ; mov [esi+24h],eax ; jge join` (a negative step is NOT stored and the wrap/clamp block after `jge` is unreachable), the lane emits `dec dword ptr [esi+24h] ; jns join` with a live wrap block. Tried: `--index < 0`, a literal `index = h - 1; if (index >= 0) { h = index; if (h < 0) {wrap} }` and an inline setter (both fold the dead block away, 87v140 / 95v140). Retail behaviour (no wrap ever) is what the test documents
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD0F7, length 140 bytes; size-optimised TU (`push 14h ; pop ecx`,
// `and dword ptr [esi+24h],0`, pass-through `push dword ptr [esi+24h]`).
// Callees: CCategory::GetPreviousOccupiedSlot @ 0x005BCFE2, CInventoryCategoryDef virtual slot 1 (+0x04, destroy),
// CTCInventoryBase virtual slot 36 (+0x90, the highlight-changed notification with the item type).
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

struct CTCInventoryBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void OnHighlightChanged(long itemType);   // slot 36 (+0x90)
};

struct CCategory_Retail {
    unsigned char     PAssociatedThing[0x8];  // +0x00 CIntelligentPointer<CThing>
    CItemVector       ItemsInCategory;        // +0x08
    CDefPointer       PDef;                   // +0x14
    CTCInventoryBase* PInventory;             // +0x18
    long              GlobalDefIndex;         // +0x1c
    long              SelectedItemIndex;      // +0x20
    long              HighlightedItemIndex;   // +0x24
    bool              Selected;               // +0x28
    bool              DisplayItems;           // +0x29
    long GetPreviousOccupiedSlot(long index); // 0x005BCFE2
    void HighlightPreviousItem();
};

void CCategory_Retail::HighlightPreviousItem()
{
    long oldIndex = HighlightedItemIndex;
    if (PDef->SelectEmptySlots) {
        if (--HighlightedItemIndex < 0) {
            if (PDef->WrapHighlightCursor)
                HighlightedItemIndex = (long)ItemsInCategory.size() - 1;
            else
                HighlightedItemIndex = 0;
        }
    } else {
        HighlightedItemIndex = GetPreviousOccupiedSlot(HighlightedItemIndex);
    }
    if (HighlightedItemIndex != oldIndex)
        PInventory->OnHighlightChanged(ItemsInCategory[HighlightedItemIndex].ItemType);
}
