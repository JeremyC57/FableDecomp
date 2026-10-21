// Behaviour test for 007cdfe0: src/007cdfe0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCBurntEffect::CTCBurntEffect(CThing* parent)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

class CThing;
class CBaseObject;
class CTCBase;

struct CCharString {
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
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
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

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

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
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
void CTCBase::v00() {}
void CTCBurntEffect::v00() {}
static char g_log[64]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 63) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
#include <stdlib.h>
static const char* g_name; static long g_nameLen; static int g_strDtor = 0;
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; g_name = text; g_nameLen = length; Log('s'); }
CCharString::~CCharString() { ++g_strDtor; Log('~'); }
static long g_classIndex; static const CCharString* g_className;
long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& className, long classIndex) const
{ g_className = &className; g_classIndex = classIndex; Log('I'); return 4242; }
static long g_defIndex; static void* g_defOut;
static unsigned char g_defObject[16];
template<class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{ g_defIndex = index; g_defOut = out; out->Object = (const T*)g_defObject; Log('D'); return true; }
static CGameDefinitionManager g_mgr;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { Log('g'); return g_mgr; }
CTCBase::CTCBase(CThing* parent) : ParentThing(parent) { Log('B'); }
static unsigned long g_reserved = 0xffffffff;
template<class T> void CRetailVector<T>::reserve(unsigned long n) { g_reserved = n; Log('R'); }
static CThing* g_appended[8]; static int g_appendCount = 0;
template<class T> void CRetailVector<T>::AppendThing(CThing* thing) { g_appended[g_appendCount++ & 7] = thing; Log('a'); }
static int g_carried = 0;
static CThing* g_items[3] = { (CThing*)0x100, (CThing*)0x200, (CThing*)0x300 };
void CTCCarrying::GetCarriedThings(CRetailVector<CThing*>& things)
{
    things._M_start = (CThing**)malloc(g_carried * sizeof(CThing*) + 4);
    for (int i = 0; i < g_carried; ++i) things._M_start[i] = g_items[i];
    things._M_finish = things._M_start + g_carried; things._M_end_of_storage._M_data = things._M_finish;
    Log('C');
}
static CTCCarrying g_carrying;
static CInterfaceMap::SEntry g_entries[2] = { { 0x46, (CTCBase*)&g_carrying }, { 0, 0 } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { Log('L'); return *type == 0x46 ? &g_entries[0] : last; }
static int g_reset = 0;
void CTCBurntEffect::Reset() { ++g_reset; Log('r'); }
int main()
{
    static CThing thing; memset(&thing, 0, sizeof thing);
    thing.m_interfaces.first = &g_entries[0]; thing.m_interfaces.last = &g_entries[1];
    // no carrying component
    {
        CTCBurntEffect* e = new CTCBurntEffect(&thing);
        CHECK(e->ParentThing == &thing); CHECK(e->CurTime == 0); CHECK(e->FadeInTime == 0); CHECK(e->FadeOutTime == 0);
        CHECK(strcmp(g_name, "SPECIAL_ABILITIES_BURNT_EFFECT_DEF") == 0); CHECK(g_nameLen == -1); CHECK(g_classIndex == 1); CHECK(g_defIndex == 4242);
        CHECK(g_defOut == &e->PDef); CHECK((const void*)e->PDef.Object == g_defObject);
        CHECK(e->TintList._M_start == 0); CHECK(g_reset == 1); CHECK(g_strDtor == 1);
        CHECK(strcmp(g_log, "BsgI~gDr") == 0);
    }
    // carrying three things: reserve(3) then each appended in order
    g_logn = 0; g_log[0] = 0;
    thing.m_presentBits[2] = 1UL << (0x46 & 31); g_carried = 3;
    {
        CTCBurntEffect* e = new CTCBurntEffect(&thing);
        CHECK(g_reserved == 3); CHECK(g_appendCount == 3);
        CHECK(g_appended[0] == g_items[0]); CHECK(g_appended[1] == g_items[1]); CHECK(g_appended[2] == g_items[2]);
        CHECK(strcmp(g_log, "BsgI~gDLCRaaar") == 0);
        CHECK(e->ParentThing == &thing);
    }
    // carrying nothing
    g_logn = 0; g_log[0] = 0; g_carried = 0; g_appendCount = 0;
    {
        CTCBurntEffect* e = new CTCBurntEffect(&thing);
        CHECK(g_reserved == 0); CHECK(g_appendCount == 0); CHECK(strcmp(g_log, "BsgI~gDLCRr") == 0); CHECK(e != 0);
    }
    if (fails == 0) printf("007cdfe0_TEST PASS\n"); else printf("007cdfe0_TEST FAIL\n");
    return fails;
}