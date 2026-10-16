// CTCInventoryBase::SelectItemWithDefIndex(long) -- finds the category of a thing def's inventory item and selects that item type in it
// Leaf: SelectItemWithDefIndex
// Grade: DIFFER(127v119) -- (1) the predicate word is pushed from memory (`push [eax+3Ch]`, item moved to ebx after) where retail loads ebx = item, eax = [ebx+3Ch] and pushes eax; (2) the thing-def CDefPointer dtor compiles as `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],0` where retail has `mov ecx,[ebp-4] ; test ecx,ecx ; je ; dec [ecx+4] ; jne`. Tried the end pointer re-read vs a `last` local (the local fixes the edi end), a pointer-local dtor, an inline Release() member, a named identifier local
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

struct CInventoryItemDef : CRefCounted {
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

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { T* object = Object; if (object && --object->RefCount == 0) object->Destroy(); }
    T* operator->() const { return Object; }
};

struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get(); // 0x0044C6B0
    bool GetDef(long index, CDefPointer<CThingDef>* out);              // 0x0042AF3C
};

namespace NInventory {

struct CCategory {                                   // retail 0x2C bytes
    unsigned char _pad_0x00[0x2C];
    void SelectItemOfType(long defIndex);            // 0x005BDEC1
};

struct CCategoryHasIdentifier {
    long Identifier;
    CCategoryHasIdentifier(long identifier) : Identifier(identifier) {}
};

CCategory* __fastcall find_if(CCategory* first, CCategory* last, CCategoryHasIdentifier pred);   // 0x005C08D3

struct CTCInventoryBase {
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
    CCategory* last = Categories_last;
    CCategory* found = find_if(Categories_first, last, CCategoryHasIdentifier(item->CategoryIdentifier));
    if (found != last)
        found->SelectItemOfType(defIndex);
    if (--item->RefCount == 0)
        item->Destroy();
}

}
