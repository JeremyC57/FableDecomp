// std::adjacent_find<ENavigatorType*,equal_to<ENavigatorType> >(ENavigatorType* first, ENavigatorType* last, equal_to<ENavigatorType> pred) -- the first position whose element equals the next one, else last
// Leaf: ??$adjacent_find@PAW4ENavigatorType@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00671BB0, 41 bytes (+ int3 padding to 48), `__fastcall` (first in ecx, last in edx,
//   the empty predicate by value on the stack, `ret 4`). Only caller: 0x00671772 (STLport `unique`'s first step, inline).
//   Context: CThingCreatureDef::Transfer (the 0x0067175D..0x00671793 run) sorts the def's NavigatorTypes vector and
//   erases the duplicates: sort 0x006720B0, adjacent_find 0x00671BB0, __unique_copy 0x00671BE0 (landed), erase 0x00672070.
// Label correction: the catalog "_Unique<ENavigatorType*>" names the caller; this is STLport adjacent_find.
// Shape: STLport verbatim `if (first == last) return last; next = first; while (++next != last) { if (pred(*first, *next))
//   return first; first = next; } return last;` with the inline equal_to. No pragma.
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum (signed compares)
namespace std {
template <class _Tp> struct equal_to {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x == __y; }
};
template <class _ForwardIter, class _BinaryPredicate>
_ForwardIter __fastcall adjacent_find(_ForwardIter __first, _ForwardIter __last, _BinaryPredicate __binary_pred)
{
    if (__first == __last)
        return __last;
    _ForwardIter __next = __first;
    while (++__next != __last) {
        if (__binary_pred(*__first, *__next))
            return __first;
        __first = __next;
    }
    return __last;
}
template ENavigatorType* __fastcall adjacent_find(ENavigatorType*, ENavigatorType*, equal_to<ENavigatorType>);
}