// std::__copy(_Pair* first, _Pair* last, _Pair* result, const random_access_iterator_tag&, int*) -- (std::pair<EClothingSuitPart, NUISystem::CComponent*>) STLport counted copy: assign last - first pairs forward, return the advanced result
// Leaf: ??$__copy@PAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@std@@PAU12@H@std@@YIPAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ba371, 37 bytes, __fastcall (first in ecx, last in edx; result, the tag reference and the
//   Distance* on the stack), `ret 0Ch`; size-optimised TU (unaligned address).
// Identity (proven): bound ??$__copy@PAU?$pair@W4EClothingSuitPart@@PAVCComponent@NUISystem@@@std@@PAU12@H@std@@... ; callers the landed std::vector<pair<EClothingSuitPart, NUISystem::CComponent*> >::erase 0x005BAAE3 and 0x005BAFE3.
// Callees: none.
// Shape (family L014): STLport 4.6 _algobase.h `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)`
//   verbatim: `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }`;
//   the pair's implicit assignment is two dword moves through esi.
#pragma optimize("s", on)
enum EClothingSuitPart { };
namespace NUISystem { class CComponent; }
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
typedef std::pair<EClothingSuitPart, NUISystem::CComponent*> _Pair;
template _Pair* __fastcall std::__copy<_Pair*, _Pair*, int>(_Pair*, _Pair*, _Pair*, const std::random_access_iterator_tag&, int*);