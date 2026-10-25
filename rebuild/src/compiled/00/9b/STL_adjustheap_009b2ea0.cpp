// std::__adjust_heap(CWideString* first, int holeIndex, int len, CWideString val, less<CWideString> comp) -- heap sift-down over wide strings: walk the larger child down to a leaf, then __push_heap val back up from there
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B2EA0, 138 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: first in ecx,
// holeIndex in edx, len, the 4-byte CWideString value and the empty comparator on the stack). Catalog name
// "_Adjust_heap<CWideString*,int,CWideString>" (Dinkumware spelling). Callers: 0x009B3A6C, 0x009B3BE3 (__make_heap
// 0x009B3BB0, this chunk), 0x009B3D9C, 0x009B488C, 0x009B5AD1, 0x009B6A98.
// Callees: CWideString::operator< @ 0x0099AF70, operator= @ 0x0099B7D0, CWideString(const CWideString&) @ 0x0099B720
// (val rebuilt in __push_heap's argument area), __push_heap @ 0x009B1C30 (this chunk), ~CWideString @ 0x0099B510.
// Shape: STLport 4.6 verbatim; topIndex lives in the `push ecx` frame slot across the loop.
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
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x009B1C30

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
}

template void __fastcall std::__adjust_heap<CWideString*, int, CWideString, std::less<CWideString> >(
    CWideString* __first, int __holeIndex, int __len, CWideString __val, std::less<CWideString> __comp);