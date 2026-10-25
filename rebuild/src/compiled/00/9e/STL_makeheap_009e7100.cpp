// std::__make_heap(pair<CCharString,CSymbolInfo>* first, pair<CCharString,CSymbolInfo>* last, CCompareString comp, pair<CCharString,CSymbolInfo>*, int*) -- STLport make_heap core over the symbol-map entries: sift every parent down from (len-2)/2 to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7100, length 122 bytes (`ret 0Ch`: comparator, value-type and distance-type pointers).
// Label correction: the catalog's "MakeHeap_Start" is STLport `__make_heap` for pair<CCharString, CSymbolMap::CSymbolInfo>*
//   with CCompareString<CSymbolMap::CSymbolInfo> (called by __partial_sort 0x009E7B40 through the inline make_heap).
// Callees: __adjust_heap @ 0x009E6DB0 (`__fastcall`: first, holeIndex in ecx/edx; len, the 16-byte value and the comparator on
//   the stack), CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x3, the by-value copy in the argument area).
// Convention: /Gr algorithm TU, `__fastcall` (first in ecx, last in edx).
// Shape: STLport 4.x verbatim: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the value is
//   STLport's explicit memberwise pair copy built in the argument area (key string, then CSymbolInfo's implicit copy:
//   two strings and the Val dword through a pointer at the element's +0xC); `first` lives in the `push ecx` frame word.
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
struct CCompareString {
    bool operator()(const SSymbolEntry& a, const SSymbolEntry& b) const { return a.first.FastCompare(b.first); }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);   // 0x009E6DB0

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
}

template void __fastcall std::__make_heap(SSymbolEntry*, SSymbolEntry*, CCompareString<CSymbolMap::CSymbolInfo>, SSymbolEntry*, int*);