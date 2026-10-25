// std::__unguarded_linear_insert(CWideString* last, CWideString val, less<CWideString> comp) -- insertion-sort inner step over wide strings: shift larger strings up one slot until val fits, then assign val there
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B1870, 74 bytes, `ret 8`: `__fastcall` (/Gr algorithm TU: last in ecx, the 4-byte
// CWideString value and the empty comparator on the stack). Catalog name "CWideString_BackupAndRestore" is descriptive and
// wrong. Callers: __linear_insert @ 0x009B58FF (landed 0x009B58B0) and the __insertion_sort @ 0x009B6972 (0x009B68D0).
// Callees: CWideString::operator<(const CWideString&) const @ 0x0099AF70 (manifest "_Iterator_base12::_Getcont" is a
// propagated label), CWideString::operator= @ 0x0099B7D0, CWideString::~CWideString @ 0x0099B510 (the by-value val).
// Shape: STLport 4.6 verbatim: `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; }
// *last = val;` with less<CWideString> inlined to the member operator< on val's argument slot.
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
}

template void __fastcall std::__unguarded_linear_insert<CWideString*, CWideString, std::less<CWideString> >(
    CWideString* __last, CWideString __val, std::less<CWideString> __comp);