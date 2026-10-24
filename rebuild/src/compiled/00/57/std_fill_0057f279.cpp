// std::fill(CHeroMarriageInfo* first, CHeroMarriageInfo* last, const CHeroMarriageInfo& val) -- STLport fill of a range of 24-byte marriage records
// Leaf: ??$fill
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057F279, 31 bytes, __fastcall (first ecx, last edx) `ret 4`. Called by the
//   CHeroMarriageInfo vector's range insert (0x00580928; see 0x0057F29E for the retail vector). The catalog
//   "_Fill<CHeroMarriageInfo*,...>" names the element; the spelling is STLport's `fill`.
// Shape: STLport 4.x verbatim: `for (; __first != __last; ++__first) *__first = __val;` -- the implicit 24-byte
//   assignment as `push 6 ; pop ecx ; rep movsd` (size TU), the pointer test scheduled between them.
#pragma optimize("s", on)
#include "engine/CHeroMarriageInfo.h"

namespace std {

template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill(CHeroMarriageInfo*, CHeroMarriageInfo*, const CHeroMarriageInfo&);

}