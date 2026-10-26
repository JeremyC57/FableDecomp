#pragma optimize("s", on)
// CCategory::CCategory(const CCategory&) -- copy constructor: intelligent pointer copy, item vector copy, def pointer copy (AddRef), then the plain members
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2086, length 82 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: CIntelligentPointer<const CThing>::CIntelligentPointer(const CIntelligentPointer&) @ 0x004A9730, vector<CItem>::vector(const vector&) @ 0x005C140E (manifest "CExpressionSet::Add", propagated).
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory view below carries the PDB names at the retail offsets.
// The CDefPointer copy is inline: store the object pointer, then AddRef it when non-null (retail stores before the null test).
class CThing;

struct CIntelligentPointer {                     // CIntelligentPointer<const CThing>, 8 bytes
    void* m_pObject;
    void* m_pInfo;
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x004A9730
};

struct CItemVector {                             // retail std::vector<CItem>
    void* first;
    void* last;
    void* end_of_storage;
    CItemVector(const CItemVector& other);       // 0x005C140E
};

struct CInventoryCategoryDef_Retail {
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long m_refCount;                             // +0x04
    void AddRef() { ++m_refCount; }
};

struct CDefPointer {                             // CDefPointer<const CInventoryCategoryDef>
    CInventoryCategoryDef_Retail* m_pObject;
    CDefPointer(const CDefPointer& other) : m_pObject(other.m_pObject) { if (m_pObject) m_pObject->AddRef(); }
};

struct CTCInventoryBase;

struct CCategory {
    CIntelligentPointer PAssociatedThing;        // +0x00
    CItemVector         ItemsInCategory;         // +0x08
    CDefPointer         PDef;                    // +0x14
    CTCInventoryBase*   PInventory;              // +0x18
    long                GlobalDefIndex;          // +0x1C
    long                SelectedItemIndex;       // +0x20
    long                HighlightedItemIndex;    // +0x24
    bool                Selected;                // +0x28
    bool                DisplayItems;            // +0x29
    CCategory(const CCategory& other);
};

CCategory::CCategory(const CCategory& other)
    : PAssociatedThing(other.PAssociatedThing)
    , ItemsInCategory(other.ItemsInCategory)
    , PDef(other.PDef)
    , PInventory(other.PInventory)
    , GlobalDefIndex(other.GlobalDefIndex)
    , SelectedItemIndex(other.SelectedItemIndex)
    , HighlightedItemIndex(other.HighlightedItemIndex)
    , Selected(other.Selected)
    , DisplayItems(other.DisplayItems)
{
}