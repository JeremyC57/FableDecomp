// std::__copy(EHeroAbility* first, EHeroAbility* last, EHeroAbility* result, const random_access_iterator_tag&, int*) -- STLport counted random-access copy of 4-byte enum elements; returns the advanced result
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F7B76, 37 bytes, `ret 0Ch` (/Gr TU: first in ecx, last in edx; result,
//   the const-ref iterator tag and the null Distance* on the stack).
// Callers: the `__copy_ptrs(first, last, result, __false_type)` forwarders 0x005F823B / 0x005F8750 and
//   vector<EHeroAbility>::erase(first, last) 0x005F8F89 (CTCInventoryAbilities TU).
// Label correction: the catalog `_Copy_impl<CRGBColour*,CRGBColour*>` is a propagated Dinkumware spelling; the body is
//   STLport `__copy(first, last, result, random_access_iterator_tag, _Distance*)`. An enum is not a POD for STLport's
//   type traits, so vector<EHeroAbility>::erase reaches this counted loop instead of the memmove `__copy_trivial`.
// Shape: size TU (`#pragma optimize("s", on)`): the result stays in its argument slot and is bumped in memory
//   (`mov esi,[esp+8] ; add dword ptr [esp+8],4`), the count runs down in edx, the slot is returned.
#pragma optimize("s", on)

enum EHeroAbility { ABILITY_NULL = 0, EHeroAbility_Force32 = 0x7fffffff };

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template EHeroAbility* __fastcall __copy<EHeroAbility*, EHeroAbility*, int>(EHeroAbility*, EHeroAbility*, EHeroAbility*,
                                                                            const random_access_iterator_tag&, int*);
}