// std::fill(C3DAnimationSequenceData::C3DVectorWord* first, C3DAnimationSequenceData::C3DVectorWord* last, const C3DVectorWord& val) -- assign val to every packed position of [first, last)
// Leaf: ??$fill@PAVC3DVectorWord@C3DAnimationSequenceData@@V12@@std@@YIXPAVC3DVectorWord@C3DAnimationSequenceData@@0ABV12@@Z
// Name fix (NF382): was the fill<C3DTriangle2*> instance (FableWin: called from vector<C3DTriangle2>::_Insert_n in
//   tc_spot_light.obj, not from this address's caller). Both call sites are in the landed
//   vector<C3DAnimationSequenceData::C3DVectorWord>::_M_fill_insert 0x00A552B0 (reached from
//   C3DAnimationSequenceData::InitialiseFromFileData 0x00A4EFC0), and FableWin vector<C3DVectorWord>::_Insert_n calls
//   fill<C3DVectorWord*> (lib_3d_animation_2.obj). Same bytes.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A52080, length 45 bytes (+3 int3 pad), `ret 4`.
// Callers: 0x00A55350 and 0x00A553AB (the vector<C3DVectorWord> fill-insert helper). No callees.
// Element (Ego_r.pdb): C3DAnimationSequenceData::C3DVectorWord = short X / Y / Z (6 bytes; engine/C3DAnimationSequenceData.h).
// Shape: STLport `fill` verbatim in a /Gr TU (`__fastcall` template: first in ecx, last in edx, &val on the stack);
//   `for (; first != last; ++first) *first = val;` -- the implicit operator= of the 6-byte array record is a block copy
//   through esi/edi (dword then word, the source pointer reloaded each pass); the /O2 loop-head pad.
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_READ
#include "engine/C3DAnimationSequenceData.h"

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill(C3DAnimationSequenceData::C3DVectorWord*, C3DAnimationSequenceData::C3DVectorWord*,
                               const C3DAnimationSequenceData::C3DVectorWord&);
}