// std::__introsort_loop(pair<CCharString,CPersistInfo>* first, pair<CCharString,CPersistInfo>* last, pair<CCharString,CPersistInfo>*, int depth_limit, CKeyPairCompareLess<CCharString,CPersistInfo> comp) -- STLport introsort recursion over the quest-persistence entries (ordered by quest-name key): while more than 16 entries remain, fall back to a partial (heap) sort at depth 0, else partition around the median-of-three pivot, recurse on the upper part and loop on the lower
// Instantiation: std::__introsort_loop<pair<CCharString,CPersistInfo>*,pair<CCharString,CPersistInfo>,int,CKeyPairCompareLess<CCharString,CPersistInfo> >
// Leaf: ??$__introsort_loop@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@U12@HV?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@std@@YIXPAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@0@00HV?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BBA40, 247 bytes, __fastcall (first ecx, last edx; the value-type tag
//   pointer, depth_limit and the empty comparator slot on the stack), `ret 0Ch`; speed TU (12-byte count by `imul 2AAAAAABh`).
// Identity (proven): (1) STLport 4.6 _algo.c __introsort_loop verbatim (__stl_threshold 16; partial_sort(first, last, last)
//   at depth 0; the by-value _Tp(__median(...)) pivot built in the argument area by three CCharString copies; the
//   recursive call on [cut, last) with a null tag pointer); (2) its only callers are itself and sort 0x004BBB40 (landed
//   ??$sort@PAU?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@V?$CKeyPairCompareLess@...),
//   whose landed body declares this callee with this exact signature; (3) the callees are the landed
//   __median 0x004B9FB0, __unguarded_partition 0x004BA080 and __partial_sort 0x004BB810 over the same element (hand457).
// Label: the catalog's "std::introsort_Recursive" is descriptive; the binding ?IntrosortLoop_004bba40@@YIXPAUSElement12@@00JUSCompare@@@Z
//   is an invented placeholder spelling.
// Callees: __median 0x004B9FB0, CCharString::CCharString(const CCharString&) 0x0099EC30 (x3, the pivot copy),
//   __unguarded_partition 0x004BA080 (callee-destroyed by-value pivot), __partial_sort 0x004BB810.
// Shape: partial_sort (the 4-argument front end) is inline; the median / partition / __partial_sort stay out of line.
class CCharString {
public:
    void* PStringData;                                      // +0x0 the shared string block
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    bool operator<(const CCharString& other) const;
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                    // Ego_r.pdb, 8 bytes
    public:
        CCharString QuestName;                              // +0x0
        CCharString PersistScript;                          // +0x4
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x0
    _T2 second;                                             // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo CPersistInfo;
typedef std::pair<CCharString, CPersistInfo> CPersistEntry;
typedef CKeyPairCompareLess<CCharString, CPersistInfo> CPersistLess;

namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);          // 0x004B9FB0
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                                       // 0x004BA080
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                                           // 0x004BB810

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }   // _STLP_VALUE_TYPE for pointers

template <class _RandomAccessIter, class _Compare>
inline void __fastcall partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                                    _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

#define __stl_threshold 16

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop(CPersistEntry*, CPersistEntry*, CPersistEntry*, int, CPersistLess);
}