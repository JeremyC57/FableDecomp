// CCategory::AddItem(CThing* item, bool select, long data) -- adds an item thing (type = its word at +0x8c) to the slot holding that type (else the first empty slot), moves a highlight resting on an empty slot there, and optionally selects it and tells the inventory
// Leaf: AddItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF5A4, 176 bytes (`ret 0Ch`); size-optimised TU with an ebp frame.
// Callees: CCategory::GetSlotWithItemsOfType @ 0x005BE85B, GetFirstEmptySlot @ 0x005BD96B, SetSelectedItemIndex
//          @ 0x005BC479 (all landed), NInventory::CItem IsFull @ 0x005BDE50 and the thing add @ 0x005BF556,
//          CTCInventoryBase virtual slot 33 (+0x84, item selected).
// Shape: a found slot is tested for full inside its branch and again (with the slot byte offset in ebx) after the
//          empty-slot fallback joins; the slot-33 call passes the address of an empty tag temporary that VC places
//          in the dead top byte of the `select` argument slot (`lea edx,[ebp+0Fh]`); the tag needs a user ctor or
//          the temporary is zeroed (`stosb`).
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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    struct CEmptyContext { CEmptyContext() { } };   // empty tag: a user ctor so the temporary is not zeroed
    virtual void OnItemSelected(long itemType, const CEmptyContext& context);   // slot 33 (+0x84)
};

class CThing { public:                               // only the item-type word is read here
    unsigned char  _pad_0x00[0x8c];
    unsigned short m_itemType;                // +0x8c
};

struct CItem_Methods : CItem {
    bool IsFull();                            // 0x005BDE50
    void Add(CThing* item, long data);         // 0x005BF556
};
struct CItemMethodsVector {                   // retail std::vector<NInventory::CItem>, 12 bytes
    CItem_Methods* first;
    CItem_Methods* last;
    CItem_Methods* end_of_storage;
    CItem_Methods* begin() const { return first; }
    CItem_Methods& operator[](unsigned long n) { return *(begin() + n); }   // STLport form
};

struct CCategory_Retail {
    unsigned char      PAssociatedThing[0x8]; // +0x00 CIntelligentPointer<CThing>
    CItemMethodsVector ItemsInCategory;       // +0x08
    CDefPointer        PDef;                  // +0x14
    CTCInventoryBase*  PInventory;            // +0x18
    long               GlobalDefIndex;        // +0x1c
    long               SelectedItemIndex;     // +0x20
    long               HighlightedItemIndex;  // +0x24
    long GetSlotWithItemsOfType(long itemType);   // 0x005BE85B
    long GetFirstEmptySlot();                 // 0x005BD96B
    void SetSelectedItemIndex(long index);    // 0x005BC479
    bool AddItem(CThing* item, bool select, long data);
};

bool CCategory_Retail::AddItem(CThing* item, bool select, long data)
{
    long slot = GetSlotWithItemsOfType(item->m_itemType);
    if (slot != -1) {
        if (ItemsInCategory[slot].IsFull())
            return false;
    } else {
        slot = GetFirstEmptySlot();
        if (slot == -1)
            return false;
    }
    if (ItemsInCategory[slot].IsFull())
        return false;
    ItemsInCategory[slot].Add(item, data);
    if (ItemsInCategory[HighlightedItemIndex].NumberOfItems == 0)
        HighlightedItemIndex = slot;
    if (select) {
        SetSelectedItemIndex(slot);
        PInventory->OnItemSelected(item->m_itemType, CTCInventoryBase::CEmptyContext());
    }
    return true;
}