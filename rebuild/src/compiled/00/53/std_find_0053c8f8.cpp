// std::find<const unsigned long*, unsigned long>(first, last, const value&) -- STLport 4.6 _algobase.h find: forward to the category-dispatched __find (0x00535F5A, out of line)
// Leaf: ??$find@PBKK@std@@YIPBKPBK0ABK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053C8F8, 19 bytes, `__fastcall` COMDAT (first in ecx, last in edx, &val on the stack, `ret 4`):
//   pushes the empty random_access_iterator_tag's address and &val, calls __find (0x00535F5A, `ret 8`).
// Name (NF70, q10815 / q10816): the entry spelling, the STLport find instance (same form as the landed find<EHeroAbility*, EHeroAbility>
//   0x005F8150); was a stand-in _Umove_EWeaponClass(AllocT*, int, EWeaponClass*) calling a stand-in AllocT::construct_<addr>.
// Model: stlport/fable_containers.h (iterator tags) + stlport/fable_algorithm.h (FABLE_RETAIL_STLP_FIND: the __fastcall find
//   declaration); this row supplies find's one-line body and declares __find (out of line, landed separately).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_FIND
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"

namespace std {
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter FABLE_FASTCALL __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                        const random_access_iterator_tag&);   // 0x00535F5A (out of line)
template <class _InputIter, class _Tp>
_InputIter FABLE_FASTCALL find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}
}
template const unsigned long* FABLE_FASTCALL std::find<const unsigned long*, unsigned long>(const unsigned long*, const unsigned long*, const unsigned long&);
