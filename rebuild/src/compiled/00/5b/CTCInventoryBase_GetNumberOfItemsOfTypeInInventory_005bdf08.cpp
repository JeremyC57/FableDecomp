// CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long itemType) -- how many items of the definition `itemType` the inventory holds: looks the definition up, takes its inventory-item sub-definition, finds the category that holds that item category and asks it; 0 when any step fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDF08, length 169 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (`__fastcall`, no arguments), CDefinitionManager::GetDef(long, CDefPointer<T>*)
// @ 0x0042AF3C (the catalog's GetDef<CSpecialAbilitiesDrunkennessDef> instantiation -- byte-identical template bodies, here the plain
// definition), CDefClassBase::GetSubDef(CDefPointer<T>*) @ 0x005BADE1 (catalog GetSubDef<CAbilityDef>; here CInventoryItemDef --
// its InventoryCategory at +0x3C is what the category search keys on), the SGI find_if over Categories @ 0x005C08C0 (`__fastcall`
// /Gr template: first in ecx, last in edx, the "GlobalDefIndex == x" predicate's long on the stack; the landed GetBoughtPrice model),
// CCategory::GetNumberOfItemsOfTypeInCategory(long) @ 0x005BDE96.
// Layout: retail CTCInventoryBase Categories {first, last, end} at +0x20 (PDB +0x24, the Components vector before it is 12 bytes).
// Shape: the two definition pointers are zero-initialised out slots ([ebp-4], [ebp-8]) filled by the out-parameter template calls
// (the bool results are ignored); each is tested for null on the way in and released by hand on the way out without a null test, in
// reverse order (the landed OnCreate form of this module: `dec [p+4] ; jne ; call [vtable+4]`); VC7.1 cross-jumps the not-found and
// no-sub-def release tails. The end pointer is a named local (ebx) that feeds both the search and the "not found" compare.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long m_refCount;                             // +0x04
};
template <class T> class CDefPointer { public:          // the out slot the definition lookups fill (counted reference handed over)
    T* Object;
    T* operator->() const { return Object; }
};
class CInventoryItemDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x3c - 0x08];
    long InventoryCategory;                      // +0x3c
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CInventoryItemDef>* out);   // 0x005BADE1
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CInventoryItemDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

namespace NInventory {
class CCategory { public:                               // 0x2C bytes
    unsigned char _body[0x2C];
    long GetNumberOfItemsOfTypeInCategory(long itemType);   // 0x005BDE96
};
CCategory* __fastcall FindCategoryWithDefIndex(CCategory* first, CCategory* last, long categoryDefIndex);   // 0x005C08C0
struct CCategoryVector {                         // std::vector<CCategory> (12 bytes)
    CCategory* _M_start;
    CCategory* _M_finish;
    CCategory* _M_end_of_storage;
    CCategory* begin() { return _M_start; }
    CCategory* end() { return _M_finish; }
};
class CTCInventoryBase { public:                        // retail 0x130 bytes
    unsigned char   _pad00[0x20];
    CCategoryVector Categories;                  // +0x20
    long GetNumberOfItemsOfTypeInInventory(long itemType);
};
long CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long itemType)
{
    if (itemType > 0) {
        CDefPointer<CDefClassBase> def;
        def.Object = 0;
        CGameDefinitionManager::Get().GetDef(itemType, &def);
        if (def.Object) {
            CDefPointer<CInventoryItemDef> itemDef;
            itemDef.Object = 0;
            def->GetSubDef(&itemDef);
            CInventoryItemDef* item = itemDef.Object;
            if (item) {
                long key = item->InventoryCategory;
                CCategory* last = Categories.end();
                CCategory* category = FindCategoryWithDefIndex(Categories.begin(), last, key);
                if (category != last) {
                    long count = category->GetNumberOfItemsOfTypeInCategory(itemType);
                    if (--item->m_refCount == 0)
                        item->Destroy();
                    CDefClassBase* thingDef = def.Object;
                    if (--thingDef->m_refCount == 0)
                        thingDef->Destroy();
                    return count;
                }
                if (--item->m_refCount == 0)
                    item->Destroy();
            }
            CDefClassBase* thingDef = def.Object;
            if (--thingDef->m_refCount == 0)
                thingDef->Destroy();
        }
    }
    return 0;
}
}