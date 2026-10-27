// std::copy(const std::pair<unsigned long, unsigned long>* first, const std::pair<unsigned long, unsigned long>* last, std::pair<unsigned long, unsigned long>* result) -- STLport copy: __copy_ptrs and the random-access __copy inlined -- assign last - first elements forward, return the end of the output
// Leaf: ??$copy@PBU?$pair@KK@std@@PAU12@@std@@YIPAU?$pair@KK@0@PBU10@0PAU10@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009b3a10, 38 bytes, __fastcall (first in ecx, last in edx, result on the stack), `ret 4`.
// Identity (proven): bound (reloc_bindings) ??$copy@PBU?$pair@KK@std@@PAU12@@std@@YIPAU?$pair@KK@0@PBU10@0PAU10@@Z (three arguments: `ret 4`); its callers are the landed vector<pair<unsigned long, unsigned long> >::_M_range_insert 0x009B54E0 (twice).
// Callees: none.
// Shape (family L045, the three-argument member of the family): the same loop as the __copy_ptrs rows; only the return
//   pops one stack argument.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
}
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                          const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _InputIter, class _OutputIter>
_OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
}
}
template std::pair<unsigned long, unsigned long>* __fastcall std::copy(const std::pair<unsigned long, unsigned long>*, const std::pair<unsigned long, unsigned long>*, std::pair<unsigned long, unsigned long>*);