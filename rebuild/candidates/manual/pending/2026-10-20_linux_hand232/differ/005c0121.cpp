// Grade: DIFFER(296v297) -- register map only: retail caches the "INVENTORY_CATEGORY" literal in ebx and loads the fetched def straight into edi (shared with the count), ours caches the literal in edi and goes through eax into ebx (one extra mov); the body and frame are otherwise retail's.
// NInventory::CTCInventoryBase::OnCreate() -- builds the inventory's categories from every INVENTORY_CATEGORY def that belongs to this inventory and is added on create, selects the first one and resets the selection/animation state
// Retail 0x005C0121, 297 bytes. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0, CDefinitionManager::GetNumberOfDefsOfType(const CCharString&) 0x009AD3B0, GetGlobalIndexFromClassIndex(
// const CCharString&, long) 0x009AD390, GetDef(long, CDefPointer*) 0x005C0C99 (manifest "GetDef<CSpecialAbilities
// DrunkennessDef>" byte twin), NInventory::CCategory(CDefPointer<const CInventoryCategoryDef>, CTCInventoryBase*, long)
// 0x005BFCD9 / ~CCategory 0x005C2739, vector<CCategory>::push_back 0x005C2D32, CCategory::SetAsSelected(bool) 0x005BC934,
// the def's slot 1 on release.
// Shape: the definition manager is fetched once into a local; the type name converts implicitly at each
// use (one frame word, re-addressed with `lea`); the category count excludes the null def; each pass zeroes an out slot, fetches the def by
// its global index, and pushes a CCategory temporary built from a by-value def pointer copy (add-ref'd in its argument
// slot) when the def belongs to this inventory and is added on create; the def is released by hand without a null
// test (as the landed AddCategory 0x005C024A). Afterwards the first category is selected and the state is reset.
// Layout (retail; Ego_r.pdb names, 0x14 below the PDB past the vectors): InventorySelectionState +0x1C, Categories
// +0x20, HighlightedItemNumOwned/MaxOwned +0x2C/+0x30, CanItemBeQuickAssigned/CanItemBeUsed +0x34/+0x35, InventoryOpen
// +0x58, GlobalDefIndex +0x6C, SelectedCategoryIndex +0x70, pBuildingThing +0xB8, EnableAnimXAxis/ZAxis +0x120/+0x121,
// ParentMapNumber +0x12C; CThing map number short +0x8E; CInventoryCategoryDef Inventory +0x3C, AddCategoryOnCreate +0x54.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CThing {
    unsigned char _pad_0x00[0x8e];
    short MapNumber;                                   // +0x8e
};
struct CInventoryCategoryDef {
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+0x04)
    long RefCount;                                     // +0x04
    unsigned char _pad_0x08[0x3c - 0x08];
    long Inventory;                                    // +0x3c
    unsigned char _pad_0x40[0x54 - 0x40];
    bool AddCategoryOnCreate;                          // +0x54
};
struct CDefPointer {                                   // CDefPointer<const CInventoryCategoryDef>
    CInventoryCategoryDef* Object;
    __forceinline CDefPointer(CInventoryCategoryDef* object) : Object(object)
    {
        if (object)
            ++object->RefCount;
    }
    CDefPointer(const CDefPointer& other);
    ~CDefPointer();
};
struct CDefPointerSlot {                               // the out-parameter view of a CDefPointer (released by hand)
    CInventoryCategoryDef* Object;
};
struct CDefinitionManager {
    long GetNumberOfDefsOfType(const CCharString& typeName);                       // 0x009AD3B0
    long GetGlobalIndexFromClassIndex(const CCharString& typeName, long classIndex);   // 0x009AD390
    bool GetDef(long globalIndex, CDefPointerSlot* def);                          // 0x005C0C99
};
struct CTCInventoryBase;
namespace NInventory {
struct CCategory {
    unsigned char _body[0x2C];
    CCategory(CDefPointer def, CTCInventoryBase* inventory, long defIndex);   // 0x005BFCD9
    ~CCategory();                                      // 0x005C2739
    void SetAsSelected(bool selected);                 // 0x005BC934
};
}
struct CCategoryVector {
    NInventory::CCategory* _M_start;
    NInventory::CCategory* _M_finish;
    NInventory::CCategory* _M_end_of_storage;
    void push_back(const NInventory::CCategory& category);   // 0x005C2D32
};

struct CTCInventoryBase {
    void* __vftable;
    CThing* ParentThing;                               // +0x04
    unsigned char _pad_0x008[0x1c - 0x08];
    long InventorySelectionState;                      // +0x1c
    CCategoryVector Categories;                        // +0x20
    long HighlightedItemNumOwned;                      // +0x2c
    long HighlightedItemMaxOwned;                      // +0x30
    bool CanItemBeQuickAssigned;                       // +0x34
    bool CanItemBeUsed;                                // +0x35
    unsigned char _pad_0x036[0x58 - 0x36];
    bool InventoryOpen;                                // +0x58
    unsigned char _pad_0x059[0x6c - 0x59];
    long GlobalDefIndex;                               // +0x6c
    long SelectedCategoryIndex;                        // +0x70
    unsigned char _pad_0x074[0xb8 - 0x74];
    CThing* pBuildingThing;                            // +0xb8
    unsigned char _pad_0x0bc[0x120 - 0xbc];
    bool EnableAnimXAxis;                              // +0x120
    bool EnableAnimZAxis;                              // +0x121
    unsigned char _pad_0x122[0x12c - 0x122];
    long ParentMapNumber;                              // +0x12c
    CDefinitionManager* GetDefinitionManager() const;  // 0x006869D0 (CTCBase)
    void OnCreate();
};

void CTCInventoryBase::OnCreate()
{
    InventorySelectionState = 0;
    CDefinitionManager* definitions = GetDefinitionManager();
    long count = definitions->GetNumberOfDefsOfType("INVENTORY_CATEGORY") - 1;
    for (long i = 0; i < count; ++i) {
        long index = definitions->GetGlobalIndexFromClassIndex("INVENTORY_CATEGORY", i + 1);
        CDefPointerSlot def;
        def.Object = 0;
        definitions->GetDef(index, &def);
        CInventoryCategoryDef* category = def.Object;
        if (category->Inventory == GlobalDefIndex && category->AddCategoryOnCreate)
            Categories.push_back(NInventory::CCategory(CDefPointer(category), this, index));
        if (--category->RefCount == 0)
            category->Destroy();
    }
    SelectedCategoryIndex = 0;
    if (Categories._M_start != Categories._M_finish)
        Categories._M_start->SetAsSelected(true);
    pBuildingThing = 0;
    HighlightedItemNumOwned = 0;
    HighlightedItemMaxOwned = 0;
    CanItemBeQuickAssigned = false;
    CanItemBeUsed = false;
    EnableAnimXAxis = false;
    EnableAnimZAxis = false;
    InventoryOpen = false;
    ParentMapNumber = ParentThing->MapNumber;
}
