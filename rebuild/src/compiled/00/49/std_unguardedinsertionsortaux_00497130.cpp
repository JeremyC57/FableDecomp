// std::__unguarded_insertion_sort_aux(CWideString* first, CWideString* last, CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare comp) -- (<CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare>) insert every element of [first, last) into the sorted run before it, no lower-bound test
// Leaf: ??$__unguarded_insertion_sort_aux@PAVCWideString@@V1@VCCaseInsensitiveStringCompare@NGameComponentHelpers@@@std@@YIXPAVCWideString@@00VCCaseInsensitiveStringCompare@NGameComponentHelpers@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00497130, 46 bytes, `__fastcall` (first ecx, last edx), `ret 8`.
// Identity (probable): no caller (a dead COMDAT next to the wrapper 0x004972E0 of the same instantiation); it calls the landed __unguarded_linear_insert<CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare> 0x00496FD0 with a CWideString copy (0x0099B720) of *i; `ret 8` = the (_Tp*, comp) stack pair of the aux.
// Callees: CWideString copy constructor 0x0099B720, std::__unguarded_linear_insert<CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare> 0x00496FD0 (__fastcall, the
//   value and the empty comparator by value).
// Shape (family M072, representative 0x00497130): STLport 4.6 _algo.c `for (__i = __first; __i != __last; ++__i)
//   __unguarded_linear_insert(__i, _Tp(*__i), __comp);`; speed TU (the aligned loop's `nop`).
class CWideString {                                       // 4 bytes (PStringData)
public:
    CWideString(const CWideString& other);                // 0x0099B720
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
namespace NGameComponentHelpers {
class CCaseInsensitiveStringCompare {                     // empty; _wcsicmp(a, b) < 0 (landed 0x00496FD0)
public:
    bool operator()(const CWideString& a, const CWideString& b) const;
};
}
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00496FD0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (CWideString*)0, __comp);
}
template void __fastcall __unguarded_insertion_sort_aux<CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare >(CWideString*, CWideString*, CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare);
}