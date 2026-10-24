// std::fill<C2DCoordI*,C2DCoordI>(C2DCoordI* first, C2DCoordI* last, const C2DCoordI& value) -- assign value to every element of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654E67, 30 bytes, `__fastcall` (first in ecx, last in edx, &value on the stack,
// `ret 4`). Only caller: the two fill steps of vector<C2DCoordI>::_M_fill_insert @ 0x00656A40 (both branches share
// the one `lea eax,[ebp-8] ; push eax ; call` tail), so the element is C2DCoordI (the CDrawQuickAccessIcons
// EffectPositions vector, landed resize 0x00656FAB).
// Label correction: the catalog "_Fill<CObjectFamilyEntry*,CObjectFamilyEntry>" is a propagated Dinkumware spelling;
// retail STL is STLport, and the instance is fill<C2DCoordI*, C2DCoordI>.
// Shape: STLport `fill`: `for (; first != last; ++first) *first = value;` -- the 8-byte POD assignment copies the two
// longs through esi; the value pointer is loaded once after the empty-range test.
#pragma optimize("s", on)

class C2DCoordI {
public:
    long X;                                          // +0x00
    long Y;                                          // +0x04
};

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template void __fastcall fill<C2DCoordI*, C2DCoordI>(C2DCoordI*, C2DCoordI*, const C2DCoordI&);
}