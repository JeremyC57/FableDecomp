// std::__find_if(pair<CCharString, CSymbolMap::CSymbolInfo>* first, pair<...>* last, CSymbolGroupEquals pred, const random_access_iterator_tag&) -- STLport four-way unrolled find_if over the symbol map's entries: the first entry whose CSymbolInfo::GroupName equals the predicate's group name (CCharString equality: same data block, or same length and Compare == 0), else last
// Leaf: ??$__find_if@PAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E6A70, 432 bytes, /Gr `__fastcall` (first in ecx, last in edx), `ret 8`: the
//   4-byte predicate (one CCharString) by value, destroyed by the callee on the way out, plus the empty tag reference.
// Identity: the element is the 16-byte pair<CCharString, CSymbolMap::CSymbolInfo> of CSymbolMap's
//   CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> > (the neighbouring 0x009E6410..0x009E7C70 rows), and the
//   compared field at +4 is CSymbolInfo::GroupName (PDB layout: GroupName +0, EntryName +4, Val +8). Callers: the
//   find_if wrapper 0x009E6F30 (copies the predicate with CCharString's copy ctor 0x0099EC30) and 0x009E73AC. The catalog
//   label "ParticleMorphs_FindEntryByKey" is wrong (no CParticleMorphs::CEntry here); the predicate's own name is not in the PDB,
//   so CSymbolGroupEquals is descriptive.
// Callees: Compare(const char*, const char*) 0x00411570 (`__fastcall`, the inlined CCharString equality),
//   CCharString::~CCharString 0x0099EAE0 (the by-value predicate).
// Shape: STLport 4.6 `__find_if` verbatim (trip count loop, then the 3/2/1 switch). The predicate's data block stays in
//   esi across every Compare call: Compare's real body (landed 0x00411570) sits in the TU under `#pragma
//   auto_inline(off)` so VC7.1 knows the call writes no memory (the landed 0x004B8E40 __find model).
namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for (; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;

        if (__pred(*__first)) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}
}

class CCharStringData { public:
    char* m_pData;
    long m_length;
};
#pragma auto_inline(off)
long __fastcall Compare(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

class CSymbolMap {
public:
    class CSymbolInfo {
    public:
        CCharString GroupName;
        CCharString EntryName;
        long Val;
    };
};
typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> TSymbolEntry;

struct CSymbolGroupEquals {
    CCharString GroupName;
    bool operator()(const TSymbolEntry& entry) const { return entry.second.GroupName == GroupName; }
};

template TSymbolEntry* __fastcall std::__find_if<TSymbolEntry*, CSymbolGroupEquals>(TSymbolEntry*, TSymbolEntry*,
                                                                                  CSymbolGroupEquals,
                                                                                  const std::random_access_iterator_tag&);