#pragma optimize("s", on)
// CCategory::GetNumberOfSlots() -- reads NumberOfSlots from the category definition through a scoped CDefPointer reference
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD1CF, length 28 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: CInventoryCategoryDef virtual slot 1 (+0x04, destroy) when the reference count drops to zero. The manifest label "GetCategoryIdentifier" belongs to the sibling at 0x005BD1B3 (which reads +0x50, CategoryIdentifier); this body reads +0x40 = NumberOfSlots.
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
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

struct CCategory_Retail {
    unsigned char PAssociatedThing[0x8];         // +0x00 CIntelligentPointer<CThing>
    unsigned char ItemsInCategory[0xC];          // +0x08 vector<CItem>
    CDefPointer   PDef;                          // +0x14
    long GetNumberOfSlots();
};

long CCategory_Retail::GetNumberOfSlots()
{
    return PDef->NumberOfSlots;
}
