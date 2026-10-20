// Grade: DIFFER(301v297) -- only the two title-def releases: ours `mov eax,[ebp-4] ; test eax,eax ; je ; dec [eax+4] ; mov ecx,eax`, retail `mov ecx,[ebp-4] ; test ecx,ecx ; je ; dec [ecx+4]` (known def-release load-register gap); layout, registers and lookups match.
// NInventory::CTCInventoryBase::AddItemToInventory(CThing* item, bool silent, bool fromShop, long price, bool noTutorials) -- a hero-title item sets the owner's title instead; otherwise the item goes into the category matching its identifier and OnItemAdded is raised; true when added
// Retail 0x005BF654, 297 bytes, `ret 14h`. Frame-pointer size TU: optimize("y", off) + ("s", on).
// Callees: CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const CHeroTitleDef>*) const 0x005B1E5A (on the item's def),
// std::bitset::test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (inline GetInterface: the owner's hero component 0x29)
// and the const LowerBound 0x004365B0 (the item's inventory-item component 0x1A), CTCHero::SetTitle(long) 0x006CA400,
// CTCInventoryItem::GetCategoryIdentifier() const 0x005D91F0, FindCategory(first, last, identifier) 0x005C08D3
// (__fastcall), NInventory::CCategory::AddItem(CThing*, bool, long) 0x005BF5A4, this's slot 84 (+0x150, OnItemAdded(long,
// bool, bool) 0x005BE028), the defs' slot 1 on release.
// Shape: the title sub-def out pointer is a zeroed frame word; the item's def pointer is copied (inline add-ref) for the
// GetSubDef call only, and the bool result is homed in the dead `item` slot's high byte; the title path returns false;
// the inventory-item component pointer starts at 0 (the hit path takes it in ecx, the miss path re-reads the zero);
// both exits release the title sub-def through the pointee's inline Release().
// Layout: ParentThing +0x04, Categories +0x20 (12-byte elements view: first/last); retail CThing PDef +0x70, def index
// word +0x8C.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit) const;                       // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
    SEntry* LowerBound(const long* type) const;      // 0x004365B0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (DecRefToZero(RefCount)) Destroy(); }
};
struct CHeroTitleDef : CDefPointeeBase {};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CHeroTitleDef>* out) const;   // 0x005B1E5A
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
    unsigned char _pad_0x74[0x8c - 0x74];
    unsigned short DefIndex;                         // +0x8c
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || wanted < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CTCHero {                                     // interface type 0x29
    void SetTitle(long titleObjectDef);              // 0x006CA400
};
struct CTCInventoryItem {                            // interface type 0x1A
    long GetCategoryIdentifier() const;              // 0x005D91F0
};
namespace NInventory {
struct CCategory {
    bool AddItem(CThing* thing, bool silent, long price);   // 0x005BF5A4
};
}
NInventory::CCategory* __fastcall FindCategory(NInventory::CCategory* first, NInventory::CCategory* last, long ident);   // 0x005C08D3

namespace NInventory {
struct CTCInventoryBase {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void OnItemAdded(long itemDefIndex, bool fromShop, bool noTutorials);   // slot 84 (+0x150)
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x008[0x20 - 0x08];
    CCategory* m_categoriesFirst;                    // +0x20 Categories
    CCategory* m_categoriesLast;                     // +0x24
    bool AddItemToInventory(CThing* item, bool silent, bool fromShop, long price, bool noTutorials);
};

bool CTCInventoryBase::AddItemToInventory(CThing* item, bool silent, bool fromShop, long price, bool noTutorials)
{
    CDefPointer<CHeroTitleDef> titleDef;
    bool isTitle;
    {
        CDefPointer<CDefClassBase> def(item->PDef);
        isTitle = def->GetSubDef(&titleDef);
    }
    if (isTitle) {
        CTCHero* hero;
        if (ParentThing->GetInterface(&hero, 0x29))
            hero->SetTitle(item->DefIndex);
    } else {
        const CThing* constItem = item;
        CTCInventoryItem* inventoryItem = 0;
        constItem->GetInterface(&inventoryItem, 0x1a);
        CCategory* category = FindCategory(m_categoriesFirst, m_categoriesLast, inventoryItem->GetCategoryIdentifier());
        if (category->AddItem(item, silent, price)) {
            OnItemAdded(item->DefIndex, fromShop, noTutorials);
            return true;
        }
    }
    return false;
}
}
