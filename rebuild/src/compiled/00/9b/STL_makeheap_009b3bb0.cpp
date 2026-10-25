// std::__make_heap(CWideString* first, CWideString* last, less<CWideString> comp, CWideString*, int*) -- build a heap over wide strings: __adjust_heap every parent from the last one down to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B3BB0, 73 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// edx, the empty comparator and the two type-tag pointers on the stack). Catalog name "MakeHeap_CWideString_Build"
// (descriptive). Callers: make_heap @ 0x009B4729 and the __partial_sort @ 0x009B6A51.
// Callees: CWideString(const CWideString&) @ 0x0099B720 (the value rebuilt in __adjust_heap's argument area),
// __adjust_heap @ 0x009B2EA0 (this chunk).
// Shape: STLport 4.6 verbatim: `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
// __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }`.
class CWideString {
public:
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator<(const CWideString& other) const;     // 0x0099AF70
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x009B2EA0

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}
}

template void __fastcall std::__make_heap<CWideString*, std::less<CWideString>, CWideString, int>(
    CWideString* __first, CWideString* __last, std::less<CWideString> __comp, CWideString*, int*);