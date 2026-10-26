// std::__partial_sort(pair<CCharString,CSymbolInfo>* first, pair<CCharString,CSymbolInfo>* middle, pair<CCharString,CSymbolInfo>* last, pair<CCharString,CSymbolInfo>*, CCompareString comp) -- STLport partial_sort over the symbol-map entries: heapify [first, middle), pop in every later entry that sorts before the root, then sort_heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7B40, length 132 bytes (`ret 0Ch`: last, the value-type pointer, the comparator).
// Label correction: the catalog's "Partition_IntroSort" is STLport `__partial_sort` (the introsort's depth-limit fallback)
//   for pair<CCharString, CSymbolMap::CSymbolInfo>* with CCompareString<CSymbolMap::CSymbolInfo>.
// Callees: __make_heap @ 0x009E7100 (the inline make_heap: `push 0 ; push 0 ; push comp` = null value/distance-type
//   pointers), CCharString::FastCompare @ 0x009E6530 (the inline comparator), CCharString copy constructor @ 0x0099EC30 (x3),
//   __pop_heap @ 0x009E7060 (landed), sort_heap @ 0x009E7950 (landed).
// Convention: /Gr algorithm TU, `__fastcall` (first in ecx, middle in edx).
// Shape: STLport 4.x verbatim: `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` -- the
//   comparator is re-read from its slot after each pop (edi doubles as the copy pointer of the argument-area value).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);        // 0x0099EC30
    ~CCharString();                               // 0x0099EAE0
    bool FastCompare(const CCharString& other) const;   // 0x009E6530
};

class CSymbolMap {
public:
    class CSymbolInfo {                           // 12 bytes
    public:
        CCharString GroupName;                    // +0x00
        CCharString EntryName;                    // +0x04
        long        Val;                          // +0x08
    };
};

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) {}
};
}

typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> SSymbolEntry;

template <class T>
class CCompareString { public:
    bool operator()(const SSymbolEntry& a, const SSymbolEntry& b) const { return a.first.FastCompare(b.first); }
};

namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x009E7100

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp>
inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                           _Tp __value, _Compare __comp, _Distance*);   // 0x009E7060

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x009E7950

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

template void __fastcall std::__partial_sort(SSymbolEntry*, SSymbolEntry*, SSymbolEntry*, SSymbolEntry*, CCompareString<CSymbolMap::CSymbolInfo>);