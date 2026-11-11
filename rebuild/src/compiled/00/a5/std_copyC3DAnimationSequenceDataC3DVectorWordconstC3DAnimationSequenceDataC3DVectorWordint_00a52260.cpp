// std::__copy(C3DAnimationSequenceData::C3DVectorWord const* first, C3DAnimationSequenceData::C3DVectorWord const* last, C3DAnimationSequenceData::C3DVectorWord* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PBVC3DVectorWord@C3DAnimationSequenceData@@PAV12@H@std@@YIPAVC3DVectorWord@C3DAnimationSequenceData@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A52260, 70 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a54cb0 std::vector<C3DAnimationSequenceData::C3DVectorWord, std::allocator<C3DAnimationSequenceData::C3DVectorWord>>::operator=; caller 00a54cb0 std::vector<C3DAnimationSequenceData::C3DVectorWord, std::allocator<C3DAnimationSequenceData::C3DVectorWord>>::operator=; binder ??$__copy@PBVC3DVectorWord@C3DAnimationSequenceData@@PAV12@H@std@@YIPAVC3DVectorWord@C3DAnimationSequenceData@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DAnimationSequenceData::C3DVectorWord.
#include "stlport/fable_containers.h"
class C3DAnimationSequenceData {
public:
class C3DVectorWord {
public:                 // C3DAnimationSequenceData::C3DVectorWord, 6 bytes (quantised x/y/z)
    short X;                           // +0
    short Y;                           // +2
    short Z;                           // +4
};
};
template C3DAnimationSequenceData::C3DVectorWord* std::__copy<C3DAnimationSequenceData::C3DVectorWord const*,C3DAnimationSequenceData::C3DVectorWord*,int >(C3DAnimationSequenceData::C3DVectorWord const*, C3DAnimationSequenceData::C3DVectorWord const*, C3DAnimationSequenceData::C3DVectorWord*, std::random_access_iterator_tag const&, int*);
