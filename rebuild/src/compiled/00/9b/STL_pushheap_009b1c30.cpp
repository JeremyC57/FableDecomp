// std::__push_heap(CWideString* first, int holeIndex, int topIndex, CWideString val, less<CWideString> comp) -- heap sift-up over wide strings: move parents down while they sort before val, then assign val into the hole
// Leaf: ??$__push_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B1C30, 110 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: first in ecx,
// holeIndex in edx, topIndex, the 4-byte CWideString value and the empty comparator on the stack). Catalog name
// "_Push_heap<CWideString*,int,CWideString>" (Dinkumware spelling of the same algorithm). Caller: __adjust_heap @ 0x009B2F14
// (0x009B2EA0, this chunk).
// Callees: CWideString::operator< @ 0x0099AF70, operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510 (the by-value val).
// Shape: STLport 4.6 verbatim; `(hole - 1) / 2` is the signed cdq/sub/sar division, less<CWideString> inlined to the
// member operator< on the parent with val's argument slot as the operand.
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
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}

template void __fastcall std::__push_heap<CWideString*, int, CWideString, std::less<CWideString> >(
    CWideString* __first, int __holeIndex, int __topIndex, CWideString __val, std::less<CWideString> __comp);