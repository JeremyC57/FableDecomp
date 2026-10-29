// CTCBurntEffect::CTCBurntEffect(CThing* parent) -- constructor: CTCBase, null def, zero timers, two empty effect vectors; load SPECIAL_ABILITIES_BURNT_EFFECT_DEF, collect the things the parent carries into TintList, then Reset()
// Leaf: ??0CTCBurntEffect
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CDFE0, 284 bytes (+4 int3 padding). `ret 4`; vtable 0x0126EFBC.
// Callees: CTCBase::CTCBase(CThing*) 0x00686800; CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0;
//   CGameDefinitionManager::Get 0x0044C6B0 (__fastcall, twice); CDefinitionManager::GetGlobalIndexFromClassIndex
//   0x009AD390; CDefinitionManager::GetDef<CSpecialAbilitiesBurntEffectDef> 0x007CE100;
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (non-const); CTCCarrying member filling a
//   vector<CThing*> with the carried things 0x006A2220 (descriptive name; manifest label RemapGroups is propagated);
//   vector<CIntelligentPointer<CThing> >::reserve 0x0077E580; STLport `__copy` into a back_insert_iterator 0x007CE1F0
//   (/Gr __fastcall: hidden result in ecx, first in edx); free 0x00BFEA14; CTCBurntEffect::Reset 0x007CDD10.
// Layout (retail, 0x38 bytes = the factory 0x004D58C6's `push 38h`): CTCBase +0x00..+0x0b, PDef +0x0c, CurTime +0x10,
//   FadeInTime +0x14, FadeOutTimeStart +0x18, FadeOutTime +0x1c, Effects +0x20 and TintList +0x2c (12-byte STLport
//   vectors of 8-byte CIntelligentPointer<CThing>; Ego_r.pdb has 16-byte vectors, TintList +0x30). No engine header.
// Shape: the landed CTCDrainLife constructor (0x007C30E0) pattern: init-list zeroes after the vtable store, the def name
//   a named CCharString in its own block (homed in the dead `parent` slot, like the interface key, the copy's tag and
//   its result). Interface 0x46 is CTCCarrying (bitset byte +0x28 bit 6). The local vector<CThing*> lives in the `if`
//   block: GetCarriedThings, reserve(size()), copy(begin, end, back_inserter(TintList)), then its null-tested free.
//   The copy loop __copy (0x007CE1F0, same TU, back_insert_iterator returned by hidden pointer in ecx) has a
//   same-TU stand-in body (AppendThing = push_back of the converted CIntelligentPointer): with only a declaration
//   VC7.1 homes the hidden result in the dead `index` slot instead of the dead `parent` slot.
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class CThing;
class CBaseObject;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSpecialAbilitiesBurntEffectDef;

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
};

class CDefinitionManager {
public:
    long GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const;   // 0x009AD390
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;            // 0x007CE100 for this def
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                                     // 0x0044C6B0
};

template <class T> struct SRetailAllocProxy {       // STLport _STLP_alloc_proxy
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CRetailVector()
    {
        if (_M_start)
            free(_M_start);
    }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
    void AppendThing(CThing* thing);                // stand-in for push_back(CIntelligentPointer<CThing>(thing))
    void reserve(unsigned long n);                  // 0x0077E580 for vector<CIntelligentPointer<CThing> >
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};

struct random_access_iterator_tag {
    random_access_iterator_tag() {}
};
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& x) : container(&x) {}
};
template <class C> inline back_insert_iterator<C> back_inserter(C& x) { return back_insert_iterator<C>(x); }

template <class I, class O, class D>
__declspec(noinline) O __fastcall __copy(I first, I last, O result, const random_access_iterator_tag&, D*)
{
    for (D n = last - first; n > 0; --n) {
        result.container->AppendThing(*first);
        ++first;
    }
    return result;
}
template <class T> inline random_access_iterator_tag iterator_category(const T*) { return random_access_iterator_tag(); }
template <class T> inline long* distance_type(const T*) { return (long*)0; }
struct __false_type {
    __false_type() {}
};
template <class I, class O>
inline O __copy_aux(I first, I last, O result, const __false_type&)
{
    return __copy(first, last, result, iterator_category(first), distance_type(first));
}
template <class I, class O>
inline O copy(I first, I last, O result)
{
    return __copy_aux(first, last, result, __false_type());
}

class CTCCarrying {
public:
    void GetCarriedThings(CRetailVector<CThing*>& things);   // 0x006A2220
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];                      // ToBeRemoved, IsInGlobalUpdateTCs
};

class CTCBurntEffect : public CTCBase {             // vtable 0x0126EFBC
public:
    CDefPointer<const CSpecialAbilitiesBurntEffectDef> PDef;  // +0x0c
    long CurTime;                                   // +0x10
    long FadeInTime;                                // +0x14
    long FadeOutTimeStart;                          // +0x18
    long FadeOutTime;                               // +0x1c
    CRetailVector< CIntelligentPointer<CThing> > Effects;    // +0x20
    CRetailVector< CIntelligentPointer<CThing> > TintList;   // +0x2c
    CTCBurntEffect(CThing* parent);
    virtual void v00();
    void Reset();                                   // 0x007CDD10
};

CTCBurntEffect::CTCBurntEffect(CThing* parent)
    : CTCBase(parent), CurTime(0), FadeInTime(0), FadeOutTimeStart(0), FadeOutTime(0)
{
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_BURNT_EFFECT_DEF", -1);
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CGameDefinitionManager::Get().GetDef(index, &PDef);
    if (ParentThing->HasTC(0x46)) {
        CTCCarrying* carrying = ParentThing->GetTC<CTCCarrying>(0x46);
        CRetailVector<CThing*> things;
        carrying->GetCarriedThings(things);
        TintList.reserve(things.size());
        copy(things.begin(), things.end(), back_inserter(TintList));
    }
    Reset();
}