// CTCInventoryBase::SelectItemWithDefIndex(long) -- finds the category of a thing def's inventory item and selects that item type in it
// Leaf: SelectItemWithDefIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDFB1, 117 bytes (+2 int3); frame-pointer size TU
// (`#pragma optimize("y", off)` + `optimize("s", on)`), `ret 4`.
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (__fastcall), CDefinitionManager::GetDef<CThingBaseDef>(long,
//          CDefPointer<const CThingBaseDef>*) const @ 0x0042AF3C, CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer<const ..>*) const @ 0x005BADE1,
//          STLport find_if<CCategory*, CCategoryHasIdentifier> @ 0x005C08D3 (__fastcall: first in ecx, last in edx,
//          the one-word predicate by value), NInventory::CCategory::SelectItemOfType(long) @ 0x005BDEC1.
// Shape: the thing def is a CDefPointer local (zeroed, released by hand with a null test at the end); the item sub-def's
// pointer is read into ebx and released by hand without a null test; the predicate carries the item's category
// identifier (+0x3C); the end pointer stays in edi across the find.
// Layout: retail CTCInventoryBase Categories {first +0x20, last +0x24}; NInventory::CCategory 0x2C bytes.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRefCounted {
    virtual void v00();
    virtual void Destroy();                          // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};


template <class T> class CDefPointer { public:     // the out slot of the def lookups (released by hand at the end)
    CRefCounted* Object;                             // the pointee as its non-const base (engine/CDefPointer.h)
    CDefPointer() : Object(0) {}
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase : public CRefCounted { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005BADE1 GetSubDef<CInventoryItemDef> (Ego_r / FableWin template)
};
class CThingBaseDef : public CDefClassBase { public: };
class CInventoryItemDef : public CDefClassBase { public:
    unsigned char _pad_0x08[0x34];
    long          CategoryIdentifier;                // +0x3C
};
void* FindDef_CThingDef_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_CThingDef_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CDefinitionManager { public:
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C GetDef<CThingBaseDef> (FableWin.pdb template)
};
class CGameDefinitionManager : public CDefinitionManager { public:   // RTTI CGameDefinitionManager : CDefinitionManager
    static CGameDefinitionManager& __fastcall Get(); // 0x0044C6B0
};
void* FindDef_CGameDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
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

protected:
    void SelectItemWithDefIndex(long defIndex);      // Ego_r.pdb ?SelectItemWithDefIndex@CTCInventoryBase@NInventory@@IAEXJ@Z
};

void CTCInventoryBase::SelectItemWithDefIndex(long defIndex)
{
    CDefPointer<const CThingBaseDef> thingDef;
    CGameDefinitionManager::Get().GetDef(defIndex, &thingDef);
    CDefPointer<const CInventoryItemDef> pItem;
    thingDef->GetSubDef(&pItem);
    CInventoryItemDef* item = static_cast<CInventoryItemDef*>(pItem.Object);
    CCategoryHasIdentifier pred(item->CategoryIdentifier);
    CCategory* last = Categories_last;
    CCategory* found = find_if(Categories_first, last, pred);
    if (found != last)
        found->SelectItemOfType(defIndex);
    if (--item->RefCount == 0)
        item->Destroy();
    CRefCounted* object = thingDef.Object;
    if (object && --object->RefCount == 0)
        object->Destroy();
}

}