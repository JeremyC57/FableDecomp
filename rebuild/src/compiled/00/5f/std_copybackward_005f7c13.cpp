// std::__copy_backward(EHeroAbility* first, EHeroAbility* last, EHeroAbility* result, const random_access_iterator_tag&, int*) -- STLport counted backward copy of 4-byte enum elements; returns the moved-back result
// Leaf: ??$__copy_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F7C13, 39 bytes, `ret 0Ch` (/Gr TU: first in ecx, last in edx; result,
//   the const-ref iterator tag and the null Distance* on the stack).
// Callers: the `__copy_backward_ptrs(first, last, result, __false_type)` forwarder 0x005F82FB and the 4-byte
//   vector `_M_fill_insert` 0x005F9633 (both in the CTCInventoryAbilities TU).
// Label correction: the catalog `_Copy_backward<CDefIndex*,CDefIndex*>` is propagated; the body is STLport
//   `__copy_backward(first, last, result, random_access_iterator_tag, _Distance*)` (`*--result = *--last` per element).
//   The element is a 4-byte non-POD-for-traits word (an enum such as EHeroAbility, which STLport does not treat as
//   trivially assignable).
// Shape: size TU (`#pragma optimize("s", on)`): result is decremented in its own argument slot
//   (`sub dword ptr [esp+8],4 ; mov esi,[esp+8]`) and that slot is returned.
#pragma optimize("s", on)

enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class BidirectionalIter, class Distance>
BidirectionalIter __fastcall __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

template EHeroAbility* __fastcall __copy_backward<EHeroAbility*, EHeroAbility*, int>(EHeroAbility*, EHeroAbility*,
                                                                                     EHeroAbility*,
                                                                                     const random_access_iterator_tag&, int*);
}