// NInventory::CTCInventoryBase::AddItemToInventory(CThing& item, bool silent, bool fromShop, long price, bool noTutorials) -- a hero-title item sets the owner's title instead; otherwise the item goes into the category matching its identifier and OnItemAdded is raised; true when added
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
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
#include "fable_compiler.h"
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
class CTCBase;
// agentNF16: the stand-in CInterfaceMap struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    const value_type* LowerBound(const ETCInterfaceType& key) const;   // 0x004365B0
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound template member (compiled in the caller's TU):
// binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::value_type* CInterfaceMap::LowerBound(const ETCInterfaceType& key)
{
    value_type* it = _M_start;
    long count = _M_finish - _M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound template member (compiled in the caller's TU):
// binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) const CInterfaceMap::value_type* CInterfaceMap::LowerBound(const ETCInterfaceType& key) const
{
    const value_type* it = _M_start;
    long count = _M_finish - _M_start;
    while (count > 0) {
        long half = count / 2;
        if (it[half].first < key) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
static FABLE_FORCEINLINE bool DecRefToZero(long& count) { return --count == 0; }
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long RefCount;                                   // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (DecRefToZero(RefCount)) Destroy(); }
};
class CHeroTitleDef : public CDefPointeeBase { public:};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    FABLE_FORCEINLINE ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005B1E5A GetSubDef<CHeroTitleDef> (Ego_r.pdb member template)
};
void* FindDef_s7r_0();
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work FindDef_s7r_0 does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindDef_s7r_0 instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void* FindDef_s7r_0()
{
    long r = TmplSink_s7_0(0, 0);
    return (void*)r;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CHeroTitleDef>(CDefPointer<const CHeroTitleDef>* out) const
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad_0x4c[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;                 // +0x70
    unsigned char _pad_0x74[0x8c - 0x74];
    unsigned short DefIndex;                         // +0x8c
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || wanted < it->first)
                it = map->_M_finish;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted) const
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)((it == map->_M_finish || wanted < it->first) ? map->_M_finish : it)->second;
            return true;
        }
        return false;
    }
};
class CTCHero { public:                                     // interface type 0x29
    void SetTitle(long titleObjectDef);              // 0x006CA400
};
class CTCInventoryItem { public:                            // interface type 0x1A
    long GetCategoryIdentifier() const;              // 0x005D91F0
};
namespace NInventory {
class CCategory { public:
    bool AddItem(CThing* thing, bool silent, long price);   // 0x005BF5A4
};
}
NInventory::CCategory* __fastcall FindCategory(NInventory::CCategory* first, NInventory::CCategory* last, long ident);   // 0x005C08D3
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work FindCategory does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the FindCategory instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) NInventory::CCategory* __fastcall FindCategory(NInventory::CCategory* first, NInventory::CCategory* last, long ident)
{
    long r = TmplSink_s7_1(first, ident);   // the search over [first, last) for the category with this identifier
    return r ? (NInventory::CCategory*)r : last;
}
#pragma auto_inline(on)

namespace NInventory {
class CTCInventoryBase { public:
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
protected:
    // agentNF16: slot 80 (CTCInventoryClothing vtable 0x0124356C slot 80 = 0x005BF654, rtti-vtables.tsv) is this function;
    // Ego_r.pdb ?AddItemToInventory@CTCInventoryBase@NInventory@@MAE_NAAVCThing@@_N1J1@Z (protected virtual, CThing&).
    virtual bool AddItemToInventory(CThing& item, bool silent, bool fromShop, long price, bool noTutorials);
public:
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void OnItemAdded(long itemDefIndex, bool fromShop, bool noTutorials);   // slot 84 (+0x150)
    CThing* ParentThing;                             // +0x04
    unsigned char _pad_0x008[0x20 - 0x08];
    CCategory* m_categoriesFirst;                    // +0x20 Categories
    CCategory* m_categoriesLast;                     // +0x24
};

bool CTCInventoryBase::AddItemToInventory(CThing& item, bool silent, bool fromShop, long price, bool noTutorials)
{
    CDefPointer<const CHeroTitleDef> titleDef;
    bool isTitle;
    {
        CDefPointer<CDefClassBase> def(item.PDef);
        isTitle = def->GetSubDef(&titleDef);
    }
    if (isTitle) {
        CTCHero* hero;
        if (ParentThing->GetInterface(&hero, 0x29))
            hero->SetTitle(item.DefIndex);
    } else {
        const CThing* constItem = &item;
        CTCInventoryItem* inventoryItem = 0;
        constItem->GetInterface(&inventoryItem, 0x1a);
        CCategory* category = FindCategory(m_categoriesFirst, m_categoriesLast, inventoryItem->GetCategoryIdentifier());
        if (category->AddItem(&item, silent, price)) {
            OnItemAdded(item.DefIndex, fromShop, noTutorials);
            return true;
        }
    }
    return false;
}
}