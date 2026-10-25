// std::__unguarded_partition(pair<CCharString,CSymbolInfo>* first, pair<CCharString,CSymbolInfo>* last, pair<CCharString,CSymbolInfo> pivot, CCompareString comp) -- STLport quicksort partition step over the symbol-map entries: advance first past entries below the pivot, retreat last past entries above it, swap, until they cross; returns the cut
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E6C80, length 133 bytes (`ret 14h`: the 16-byte pivot + the 4-byte comparator).
// Label correction: the catalog's "Partition_QuickSort" is STLport `__unguarded_partition` instantiated for
//   pair<CCharString, CSymbolMap::CSymbolInfo>* with CCompareString<CSymbolMap::CSymbolInfo> -- the SLONGMap sort TU
//   (same element as the landed __unguarded_insertion_sort_aux 0x009E6D10 and __push_heap 0x009E67C0).
// Callees: CCharString::FastCompare @ 0x009E6530 (the inline comparator `a.first < b.first`), iter_swap @ 0x009E6480
//   (`__fastcall`), CCharString::~CCharString @ 0x0099EAE0 (three: the by-value pivot's EntryName, GroupName, first).
// Convention: the sort TU is /Gr -- `__fastcall` with first in ecx, last in edx, the pivot and the comparator on the stack.
// Shape: STLport 4.x verbatim (`for (;;) { while (comp(*first, pivot)) ++first; --last; while (comp(pivot, *last))
//   --last; if (!(first < last)) return first; iter_swap(first, last); ++first; }`); the comparator's operator() is
//   inline, so each test is a direct FastCompare call; the pivot's implicit destructor runs once at the single return.
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

template <class _ForwardIter1, class _ForwardIter2>
void __fastcall iter_swap(_ForwardIter1 __a, _ForwardIter2 __b);   // 0x009E6480

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> SSymbolEntry;

template <class T>
struct CCompareString {
    bool operator()(const SSymbolEntry& a, const SSymbolEntry& b) const { return a.first.FastCompare(b.first); }
};

template SSymbolEntry* __fastcall std::__unguarded_partition(SSymbolEntry*, SSymbolEntry*, SSymbolEntry, CCompareString<CSymbolMap::CSymbolInfo>);