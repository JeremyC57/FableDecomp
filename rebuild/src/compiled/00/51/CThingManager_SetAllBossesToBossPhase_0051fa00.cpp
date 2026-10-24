// CThingManager::SetAllBossesToBossPhase(CThingManager& thing_manager, const vector<CGenericVar>& params) -- script command: move every scan-visible boss creature to the boss phase given in params[0]
// Leaf: ?SetAllBossesToBossPhase@CThingManager@@
//
// Retail 0x0051FA00, 193 bytes, plain `ret` (__fastcall: thing manager in ecx (unused), params in edx); 0x1c-byte frame.
// Callees: 0x0052B300 CThingSearchTools::GetAllThings<CThingAICreature, vector<CThingAICreature*>,
//   CFilter_Bind2<CThingFilter_None, CThingFilter_HasTC> >, 0x0040F020 CVectorMap::LowerBound (the inline GetTC),
//   0x007AEDA0 CTCBoss::SetNextBossPhase(long phase, bool) (called with false), 0x00BFEA14 free.
// Identity: Ego_r.pdb/FableWin.pdb `?SetAllBossesToBossPhase@CThingManager@@CIXAAV1@ABV?$vector@VCGenericVar@@...@Z` (private
//   static __fastcall, a script/console command taking the thing manager and the parsed argument list); the catalog
//   name is right, only the class was missing. No direct callers (reached through a command table).
// Layout: CGenericVar is the Ego_r.pdb record (no STL members, offsets hold); CThing's interface bitset +0x20 and
//   CVectorMap +0x44, ThingTypeFlags +0x6C and the scan bits +0x91 are the landed retail models (0x0052B300, 0x00833710,
//   0x008E0990); g_pMainGameComponent 0x013B86A0 -> World +0x24 -> PThingSearchTools +0x20 (engine/CWorld.h).
// Lever: the GetAllThings template, vector::push_back and its STLport _M_insert_overflow (0x00515CC0) are written out in
//   the TU as the headers put them there (GetAllThings' instances sit right after this TU at 0x0052B270/0x0052B300).
//   With the bodies visible VC7.1 sees the result vector never escapes, so begin/end stay in ebp/edi across the
//   per-creature calls and the destructor frees the cached start; with GetAllThings only declared both are re-read
//   from the frame every iteration (124 v 120).
// Shape: params[0].ValSLONG (+0x0C) lives in ebx; the search filter is CFilter_Bind2(CThingFilter_None(),
//   CThingFilter_HasTC(TC_BOSS = 0xAD)), two references to an empty byte and the 0xAD word, built in the frame.
//   Each creature's boss component comes from the landed GetTCIfPresent idiom (`T* tc; if (HasTC) tc = GetTC; return
//   tc;`): HasTC tests bit 13 of the +0x34 word, GetTC copies the key into the frame word the filter used and runs the
//   lower-bound lookup; the never-written local is the esi loaded from that word before the loop (absent -> stale).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

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
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
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
    void SetNextBossPhase(long phase, bool force);      // 0x007AEDA0
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
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
    void SuspendAI(bool suspend, void* suspender);      // 0x00833A10
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
    bool operator()(const CThing& thing) const;         // 0x006A80A0
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
    CThingList* PeekAllThings();                        // 0x0049C770
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
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

enum { TC_BOSS = 0xAD };                                // CTCBoss interface type (bit 13 of the +0x34 word)

class CThingManager {
public:
    static void __fastcall SetSuspendAllAI(CThingManager& thing_manager, const std::vector<CGenericVar>& params);
    static void __fastcall SetAllBossesToBossPhase(CThingManager& thing_manager, const std::vector<CGenericVar>& params);
};
void __fastcall CThingManager::SetAllBossesToBossPhase(CThingManager& thing_manager, const std::vector<CGenericVar>& params)
{
    long phase = params[0].ValSLONG;
    std::vector<CThingAICreature*> creatures;
    g_pMainGameComponent->World->PThingSearchTools->GetAllThings<CThingAICreature>(
        CFilter_Bind2<CThingFilter_None, CThingFilter_HasTC>(CThingFilter_None(), CThingFilter_HasTC(TC_BOSS)), creatures);
    for (std::vector<CThingAICreature*>::iterator it = creatures.begin(); it != creatures.end(); ++it)
        (*it)->GetTCIfPresent<CTCBoss>(TC_BOSS)->SetNextBossPhase(phase, false);
}