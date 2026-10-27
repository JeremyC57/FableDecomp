// std::__copy(const _Pair* first, const _Pair* last, _Pair* result, const random_access_iterator_tag&, int*) -- (std::pair<long, EHeroAbility>) STLport counted copy: assign last - first pairs forward, return the advanced result
// Leaf: ??$__copy@PBU?$pair@JW4EHeroAbility@@@std@@PAU12@H@std@@YIPAU?$pair@JW4EHeroAbility@@@0@PBU10@0PAU10@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005f7c3a, 37 bytes, __fastcall (first in ecx, last in edx; result, the tag reference and the
//   Distance* on the stack), `ret 0Ch`; size-optimised TU (unaligned address).
// Identity (proven): bound ??$__copy@PBU?$pair@JW4EHeroAbility@@@std@@PAU12@H@std@@... (const source); caller the landed std::vector<pair<long, EHeroAbility> >::operator= 0x005F9074 (twice).
// Callees: none.
// Shape (family L014): STLport 4.6 _algobase.h `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)`
//   verbatim: `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }`;
//   the pair's implicit assignment is two dword moves through esi.
#pragma optimize("s", on)
enum EHeroAbility { };
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}
typedef std::pair<long, EHeroAbility> _Pair;
template _Pair* __fastcall std::__copy<const _Pair*, _Pair*, int>(const _Pair*, const _Pair*, _Pair*, const std::random_access_iterator_tag&, int*);