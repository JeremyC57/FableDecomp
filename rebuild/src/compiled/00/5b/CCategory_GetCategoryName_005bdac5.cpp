// CCategory::GetCategoryName() -- the display name of an inventory category: the associated thing's name when the category is bound to a thing, else the localised CategoryName text from the category def, else the empty wide string
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDAC5, length 136 bytes (ret 4: the CWideString result by hidden
// pointer); a frame-pointer size TU with esi/edi in use = `#pragma optimize("y", off)` + `("s", on)`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PAssociatedThing, twice), CTCInventoryBase::GetInventoryText
// @ 0x005BD273 (`__fastcall`, hidden CWideString result, the text id), CWideString::CWideString(const CWideString&)
// @ 0x0099B720 (from the global empty wide string @ 0x013BCA24), CTCInventoryBase virtual slot 76 (+0x130, the
// thing's name by hidden result), CInventoryCategoryDef virtual slot 1 (+0x04, destroy).
// Shape: CategoryName (+0x48) is tested through one scoped CDefPointer reference (released before the branch, the
// `setne bl` result kept across the release) and read again through a second one that stays alive across the text
// call (esi); the slot-76 call is the plain nested `PInventory->GetThingName(PAssociatedThing.GetPItem())`, so its
// vtable is fetched before the inner GetPItem call.
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

struct CThing;
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);    // 0x0099B720
};
extern CWideString g_EmptyWideString;         // 0x013BCA24
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
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual CWideString GetThingName(CThing* thing);   // slot 76 (+0x130)
    CWideString GetInventoryText(unsigned long textId);         // 0x005BD273
};
struct CBaseIntelligentPointer {
    void* __vftable;                          // +0x00
    CThing* m_pItem;                          // +0x04
    CThing* GetPItem() const;                 // 0x00A01B50
};

struct CCategory_Retail {
    CBaseIntelligentPointer PAssociatedThing; // +0x00 CIntelligentPointer<CThing>
    CItemVector       ItemsInCategory;        // +0x08
    CDefPointer       PDef;                   // +0x14
    CTCInventoryBase* PInventory;             // +0x18
    CWideString GetCategoryName();
};

CWideString CCategory_Retail::GetCategoryName()
{
    if (PAssociatedThing.GetPItem() == 0) {
        if (PDef->CategoryName != 0)
            return PInventory->GetInventoryText(PDef->CategoryName);
        return g_EmptyWideString;
    }
    return PInventory->GetThingName(PAssociatedThing.GetPItem());
}