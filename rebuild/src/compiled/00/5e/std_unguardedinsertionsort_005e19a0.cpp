// std::__unguarded_insertion_sort(CCharString* first, CCharString* last, std::less<CCharString> comp) -- (<CCharString*, std::less<CCharString>>) insert every element of [first, last) into the sorted run before it, no lower-bound test
// Leaf: ??$__unguarded_insertion_sort@PAVCCharString@@U?$less@VCCharString@@@std@@@std@@YIXPAVCCharString@@0U?$less@VCCharString@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E19A0, 46 bytes, `__fastcall` (first ecx, last edx), `ret 4`.
// Identity (proven): called by the std::sort chain 0x005E3A50 (and 0x005E2580); it calls 0x005E0710, bound ??$__unguarded_linear_insert@PAVCCharString@@V1@U?$less@VCCharString@@@std@@@std@@, with a CCharString copy (0x0099EC30) of *i; `ret 4` = the comparator slot only (the wrapper, the aux inlined).
// Callees: CCharString copy constructor 0x0099EC30, std::__unguarded_linear_insert<CCharString*, CCharString, std::less<CCharString>> 0x005E0710 (__fastcall, the
//   value and the empty comparator by value).
// Shape (family M072, representative 0x00497130): STLport 4.6 _algo.c `for (__i = __first; __i != __last; ++__i)
//   __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the inline wrapper `__unguarded_insertion_sort_aux(first, last, _STLP_VALUE_TYPE(first), comp)` with the aux inlined; speed TU (the aligned loop's `nop`).
class CCharString {                                       // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    bool operator<(const CCharString& other) const;
    void* PStringData;
};
namespace std {
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
}
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x005E0710
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (CCharString*)0, __comp);
}
template void __fastcall __unguarded_insertion_sort<CCharString*, std::less<CCharString> >(CCharString*, CCharString*, std::less<CCharString>);
}