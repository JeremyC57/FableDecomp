// CTCInventoryBase::SelectItemWithDefIndex(long) -- finds the category of a thing def's inventory item and selects that item type in it
// Leaf: SelectItemWithDefIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDFB1, 117 bytes (+2 int3); frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<CThingDef>(long,
//          CDefPointer*) @ 0x0042AF3C, CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer*) @ 0x005BADE1,
//          STLport find_if<CCategory*, CCategoryHasIdentifier> @ 0x005C08D3 (__fastcall: first in ecx, last in edx,
//          the one-word predicate by value), NInventory::CCategory::SelectItemOfType(long) @ 0x005BDEC1.
// Shape: the thing def is a CDefPointer local (zeroed, released with a null test at the end); the item sub-def's
// pointer is read into ebx and released by hand without a null test; the predicate carries the item's category
// identifier (+0x3C); the end pointer stays in edi across the find.
// Layout: retail CTCInventoryBase Categories {first +0x20, last +0x24}; NInventory::CCategory 0x2C bytes.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRefCounted {
    virtual void v00();
    virtual void Destroy();                          // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};

class CInventoryItemDef : public CRefCounted { public:
    unsigned char _pad_0x08[0x34];
    long          CategoryIdentifier;                // +0x3C
};

template <class T> struct CRawDefPointer {
    T* Object;
    CRawDefPointer() : Object(0) {}
};

struct CThingDef : CRefCounted {
    bool GetSubDef(CRawDefPointer<CInventoryItemDef>* out) const;     // 0x005BADE1
};
void* FindDef_CThingDef_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingDef::GetSubDef(CRawDefPointer<CInventoryItemDef>* out) const
{
    void* def = FindDef_CThingDef_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { T* object = Object; if (object && --object->RefCount == 0) object->Destroy(); }
    T* operator->() const { return Object; }
};

class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get(); // 0x0044C6B0
    bool GetDef(long index, CDefPointer<CThingDef>* out);              // 0x0042AF3C
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long index, CDefPointer<CThingDef>* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

namespace NInventory {

class CCategory { public:                                   // retail 0x2C bytes
    unsigned char _pad_0x00[0x2C];
    void SelectItemOfType(long defIndex);            // 0x005BDEC1
};

struct CCategoryHasIdentifier {
    long Identifier;
    CCategoryHasIdentifier(long identifier) : Identifier(identifier) {}
};

CCategory* __fastcall find_if(CCategory* first, CCategory* last, CCategoryHasIdentifier pred);   // 0x005C08D3

class CTCInventoryBase { public:
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x18];
    CCategory*    Categories_first;                  // +0x20 vector<CCategory>
    CCategory*    Categories_last;                   // +0x24
    CCategory*    Categories_end;                    // +0x28

    void SelectItemWithDefIndex(long defIndex);
};

void CTCInventoryBase::SelectItemWithDefIndex(long defIndex)
{
    CDefPointer<CThingDef> thingDef;
    CGameDefinitionManager::Get().GetDef(defIndex, &thingDef);
    CRawDefPointer<CInventoryItemDef> pItem;
    thingDef->GetSubDef(&pItem);
    CInventoryItemDef* item = pItem.Object;
    CCategoryHasIdentifier pred(item->CategoryIdentifier);
    CCategory* last = Categories_last;
    CCategory* found = find_if(Categories_first, last, pred);
    if (found != last)
        found->SelectItemOfType(defIndex);
    if (--item->RefCount == 0)
        item->Destroy();
}

}