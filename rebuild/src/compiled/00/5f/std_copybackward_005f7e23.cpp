// std::__copy_backward(std::pair<long, EHeroAbility>* first, std::pair<long, EHeroAbility>* last, std::pair<long, EHeroAbility>* result, const random_access_iterator_tag&, int*) -- assign [first, last) onto the range ending at result, back to front
// Leaf: ??$__copy_backward@PAU?$pair@JW4EHeroAbility@@@std@@PAU12@H@std@@YIPAU?$pair@JW4EHeroAbility@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F7E23, 47 bytes, `__fastcall` (first ecx, last edx; result, the tag and the distance
//   pointer on the stack), `ret 0Ch`.
// Identity (proven): bound ??$__copy_backward@PAU?$pair@JW4EHeroAbility@@@std@@PAU12@H@std@@... from the landed __linear_insert<pair<long, EHeroAbility>*> 0x005F9A98; also called by the landed copy_backward<pair<long, EHeroAbility>*> 0x005F93D1. The manifest's "_Copy_backward<CBrainUpdateZone*, ...>" is a propagated (later-STL) label.
// Shape (family M164, representative 0x005F7E23): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;` -- the element's implicit assignment is two dword moves. Size TU.
#pragma optimize("s", on)
enum EHeroAbility { HERO_ABILITY_NULL = 0 };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template ::std::pair<long, EHeroAbility>* __fastcall __copy_backward< ::std::pair<long, EHeroAbility>*, ::std::pair<long, EHeroAbility>*, int>(::std::pair<long, EHeroAbility>*, ::std::pair<long, EHeroAbility>*, ::std::pair<long, EHeroAbility>*, const random_access_iterator_tag&, int*);
}