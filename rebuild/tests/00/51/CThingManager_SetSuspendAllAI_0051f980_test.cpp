// Behaviour test for 0x0051F980 CThingManager::SetSuspendAllAI: every scan-visible AI creature (not the dead one, not
// the non-AI thing) gets SuspendAI(params[0].ValBool, 0); every block the found vector outgrows, and the final one, is freed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
using std::malloc; using std::memmove;
static int g_frees;
static void counted_free(void* p) { ++g_frees; std::free(p); }

class CCharString {
public:
    const char* PString;
};

// Ego_r.pdb CGenericVar (0x1c bytes; no STL members, so the PDB offsets hold in retail).
class CGenericVar {
public:
    int           _padding_;                            // +0x00
    long          Type;                                 // +0x04 EGenericVarType
    CCharString   ValString;                            // +0x08
    long          ValSLONG;                             // +0x0C
    unsigned long ValULONG;                             // +0x10
    bool          ValBool;                              // +0x14
    float         ValFloat;                             // +0x18
};

// ---- STLport 4.x vector (the pieces this TU instantiates) ---------------------------------------------------------
namespace std {
struct __true_type { __true_type() {} };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                   + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) counted_free(p); }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    typedef T* iterator;
    vector() { _M_start = 0; _M_finish = 0; _M_end_of_storage._M_data = 0; }
    ~vector() { _M_end_of_storage.deallocate(_M_start, (unsigned int)(_M_end_of_storage._M_data - _M_start)); }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
    void _M_clear() { _M_end_of_storage.deallocate(_M_start, (unsigned int)(_M_end_of_storage._M_data - _M_start)); }
    void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage._M_data = e; }
    void _M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend);
    void push_back(const T& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) T(__x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};
// vector<CThingAICreature*>::_M_insert_overflow is 0x00515CC0 in retail; the STLport body is in every TU that pushes.
template <class T>
void vector<T>::_M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    T* __new_start = _M_end_of_storage.allocate(__len);
    T* __new_finish = (T*)__copy_trivial(_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (T*)__copy_trivial(__position, _M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}

// ---- things and components (retail layout) ------------------------------------------------------------------------
class CTCBase;
class CTCBoss {
public:
    long Phase; int Calls; bool LastForce;
    void SetNextBossPhase(long phase, bool force) { Phase = phase; ++Calls; LastForce = force; }
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)                // test body: first entry whose key is not below *type
    {
        SEntry* p = first;
        while (p != last && p->type < *type) ++p;
        return p;
    }
};

class CThing {
public:
    enum { ThingTypeMask = 0 };
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 bitset<274> of present interface types
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                       // +0x6C THING_TYPE_* inheritance bits
    unsigned char _pad70[0x91 - 0x70];
    unsigned char StateBits;                            // +0x91 Dead 0x01 / IncludeInThingScans 0x04 / Activated 0x08 / InLimbo 0x20

    bool IsIncludedInThingScans() const                 // 0x0048BC40 out of line, inline here
    {
        unsigned char f = StateBits;
        unsigned char nf = ~f;
        if ((f & 0x04) && (f & 0x08) && (nf & 0x01) && !(f & 0x20))
            return true;
        return false;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (T*)sel->component;
    }
    template <class T> __forceinline T* GetTCIfPresent(long interfaceType)
    {
        T* tc;
        if (HasTC(interfaceType))
            tc = GetTC<T>(interfaceType);
        return tc;
    }
};
class CThingAICreature : public CThing {
public:
    enum { ThingTypeMask = 0x1f };
    int SuspendCalls; bool Suspended; void* LastSuspender;
    void SuspendAI(bool suspend, void* suspender) { ++SuspendCalls; Suspended = suspend; LastSuspender = suspender; }
};

template <class T> inline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
template <class T> inline T* ThingCast(CThing* thing) { return (T*)thing; }

// ---- thing filters ------------------------------------------------------------------------------------------------
class CThingFilter_None {
public:
    CThingFilter_None() {}
    bool operator()(const CThing&) const { return true; }
};
class CThingFilter_HasTC {
public:
    long Type;                                          // ETCInterfaceType
    CThingFilter_HasTC(long type) : Type(type) {}
    bool operator()(const CThing& thing) const { return thing.HasTC(Type) != 0; }
};
template <class A, class B> class CFilter_Bind2 {       // both filters, held by reference
public:
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) {}
    template <class T> bool operator()(const T& thing) const { return First(thing) && Second(thing); }
};

// ---- the search tools (the GetAllThings template is instantiated in this TU: 0x0052B270 / 0x0052B300) -------------
struct CThingList {                                     // STLport list<CThing*>: node = next, prev, value
    struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_value; };
    SNode* m_pHead;
    struct iterator {
        SNode* m_pNode;
        iterator(SNode* node) : m_pNode(node) {}
        CThing* operator*() const { return m_pNode->m_value; }
        iterator& operator++() { m_pNode = m_pNode->m_pNext; return *this; }
        bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; }
    };
    iterator begin() { return iterator(m_pHead->m_pNext); }
    iterator end() { return iterator(m_pHead); }
};
class CThingSearchTools {
public:
    CThingList* List;
    CThingList* PeekAllThings() { return List; }
    template <class TThing, class TContainer, class TFilter> void GetAllThings(const TFilter& filter, TContainer& result);
};
template <class TThing, class TContainer, class TFilter>
void CThingSearchTools::GetAllThings(const TFilter& filter, TContainer& result)
{
    CThingList* things = PeekAllThings();
    for (CThingList::iterator it = things->begin(); it != things->end(); ++it) {
        CThing* thing = *it;
        if (thing->IsIncludedInThingScans() && IsThingOfType<TThing>(thing) && filter(*(TThing*)thing))
            result.push_back(ThingCast<TThing>(thing));
    }
}

class CWorld {
public:
    unsigned char      _pad00[0x20];
    CThingSearchTools* PThingSearchTools;               // +0x20
};
class CMainGameComponent {
public:
    unsigned char _pad00[0x24];
    CWorld*       World;                                // +0x24
};
CMainGameComponent* g_pMainGameComponent;

enum { TC_BOSS = 0xAD };                                // CTCBoss interface type (bit 13 of the +0x34 word)

class CThingManager {
public:
    static void __fastcall SetSuspendAllAI(CThingManager& thing_manager, const std::vector<CGenericVar>& params);
    static void __fastcall SetAllBossesToBossPhase(CThingManager& thing_manager, const std::vector<CGenericVar>& params);
};
void __fastcall CThingManager::SetSuspendAllAI(CThingManager& thing_manager, const std::vector<CGenericVar>& params)
{
    bool suspend = params[0].ValBool;
    std::vector<CThingAICreature*> creatures;
    g_pMainGameComponent->World->PThingSearchTools->GetAllThings<CThingAICreature>(CThingFilter_None(), creatures);
    for (std::vector<CThingAICreature*>::iterator it = creatures.begin(); it != creatures.end(); ++it)
        (*it)->SuspendAI(suspend, 0);
}

static CThingAICreature g_c[6];
static CTCBoss g_boss;
static CInterfaceMap::SEntry g_entries[3];
static CThingList::SNode g_nodes[7];
static CThingList g_list;
static CThingSearchTools g_tools;
static CWorld g_world;
static CMainGameComponent g_mgc;
static void setup()
{
    std::memset(g_c, 0, sizeof(g_c));
    for (int i = 0; i < 6; ++i) { g_c[i].ThingTypeFlags = 0x1f; g_c[i].StateBits = 0x04 | 0x08; }
    g_c[3].StateBits = 0x04 | 0x08 | 0x01;   // dead: not in scans
    g_c[4].ThingTypeFlags = 0x03;            // not an AI creature
    // creature 0 and creature 5 carry a boss component (0xAD: bit 13 of word 5) in a sorted interface map
    g_entries[0].type = 0x10; g_entries[0].component = 0;
    g_entries[1].type = 0xAD; g_entries[1].component = (CTCBase*)&g_boss;
    g_entries[2].type = 0xC0; g_entries[2].component = 0;
    g_c[0].m_presentBits[5] = 1UL << 13; g_c[0].m_interfaces.first = g_entries; g_c[0].m_interfaces.last = g_entries + 3;
    g_c[5].m_presentBits[5] = 1UL << 13; g_c[5].m_interfaces.first = g_entries + 1; g_c[5].m_interfaces.last = g_entries + 2;
    // list: head node + six things
    g_list.m_pHead = &g_nodes[0];
    for (int i = 0; i < 7; ++i) g_nodes[i].m_pNext = &g_nodes[(i + 1) % 7];
    for (int i = 1; i < 7; ++i) g_nodes[i].m_value = &g_c[i - 1];
    g_tools.List = &g_list; g_world.PThingSearchTools = &g_tools; g_mgc.World = &g_world; g_pMainGameComponent = &g_mgc;
}
static std::vector<CGenericVar> make_params(CGenericVar* buf)
{
    std::vector<CGenericVar> v; v._M_start = buf; v._M_finish = buf + 1; v._M_end_of_storage._M_data = buf + 1;
    return v;
}
int main()
{
    setup();
    CGenericVar arg; std::memset(&arg, 0, sizeof(arg)); arg.ValBool = true;
    CGenericVar* buf = &arg;
    std::vector<CGenericVar> params; params._M_start = buf; params._M_finish = buf + 1; params._M_end_of_storage._M_data = buf + 1;
    CThingManager* tm = 0;
    CThingManager::SetSuspendAllAI(*tm, params);
    int ok = g_c[0].SuspendCalls == 1 && g_c[1].SuspendCalls == 1 && g_c[2].SuspendCalls == 1 && g_c[5].SuspendCalls == 1;
    ok &= g_c[3].SuspendCalls == 0 && g_c[4].SuspendCalls == 0;
    ok &= g_c[0].Suspended && g_c[5].Suspended && g_c[1].LastSuspender == 0 && g_frees == 3;   // two outgrown blocks + the final one
    arg.ValBool = false;
    CThingManager::SetSuspendAllAI(*tm, params);
    ok &= g_c[2].SuspendCalls == 2 && !g_c[2].Suspended && !g_c[0].Suspended && g_c[3].SuspendCalls == 0 && g_frees == 6;
    params._M_start = 0; params._M_finish = 0; params._M_end_of_storage._M_data = 0;
    if (!ok) { std::printf("0051f980_TEST FAIL\n"); return 1; }
    std::printf("0051f980_TEST PASS\n");
    return 0;
}