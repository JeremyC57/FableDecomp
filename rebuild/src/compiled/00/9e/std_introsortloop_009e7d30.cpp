// std::__introsort_loop(pair<CCharString,CSymbolMap::CSymbolInfo>* first, pair<CCharString,CSymbolMap::CSymbolInfo>* last, pair<CCharString,CSymbolMap::CSymbolInfo>*, long depth_limit, CCompareString<CSymbolMap::CSymbolInfo> comp) -- STLport introsort over the symbol-map entries: median-of-three pivot, partition, recurse on the right part, loop on the left, partial_sort when the depth runs out
// Leaf: ??$__introsort_loop@PAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@std@@U12@JV?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@std@@YIXPAU?$pair@VCCharString@@VCSymbolInfo@CSymbolMap@@@0@00JV?$CCompareString@VCSymbolInfo@CSymbolMap@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E7D30, 270 bytes, /Gr `__fastcall` (first ecx, last edx), `ret 0Ch` (value-type
//   pointer, depth limit, comparator).
// Identity: the one caller is the landed sort 0x009E7E40 (`__lg(n) * 2` depth); the element and comparator are the ones
//   the landed same-TU helpers prove: __unguarded_partition 0x009E6C80 and __partial_sort 0x009E7B40 (pair<CCharString,
//   CSymbolMap::CSymbolInfo>, 16 bytes; CCompareString<CSymbolInfo> comparing `first` with CCharString::FastCompare).
//   Catalog label "std::introsort_partial_16byte_quicksort" is descriptive.
// Callees: CCharString::FastCompare 0x009E6530 (the inline __median's five comparisons), CCharString copy ctor 0x0099EC30
//   (x3: the by-value pivot built in its argument slot), __unguarded_partition 0x009E6C80, __partial_sort 0x009E7B40 (the
//   inline partial_sort), itself (the right-part recursion).
// Shape: STLport 4.x `__introsort_loop` verbatim; threshold 16 folds to `((last-first)&~0xF) > 0x100`.
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

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                               // 0x009E6C80

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp);                                                   // 0x009E7B40

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last,
                         _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (SSymbolEntry*)0, __comp);
}

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > 16) {
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

template void __fastcall __introsort_loop(SSymbolEntry*, SSymbolEntry*, SSymbolEntry*, long,
                                          CCompareString<CSymbolMap::CSymbolInfo>);
}