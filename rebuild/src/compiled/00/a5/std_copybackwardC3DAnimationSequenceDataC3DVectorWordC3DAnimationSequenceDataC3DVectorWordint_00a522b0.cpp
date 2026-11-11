// std::__copy_backward(C3DAnimationSequenceData::C3DVectorWord* first, C3DAnimationSequenceData::C3DVectorWord* last, C3DAnimationSequenceData::C3DVectorWord* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVC3DVectorWord@C3DAnimationSequenceData@@PAV12@H@std@@YIPAVC3DVectorWord@C3DAnimationSequenceData@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A522B0, 71 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a552b0 std::vector<C3DAnimationSequenceData::C3DVectorWord, std::allocator<C3DAnimationSequenceData::C3DVectorWord>>::_M_fill_insert; binder ??$__copy_backward@PAVC3DVectorWord@C3DAnimationSequenceData@@PAV12@H@std@@YIPAVC3DVectorWord@C3DAnimationSequenceData@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DAnimationSequenceData::C3DVectorWord.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
class C3DAnimationSequenceData {
public:
class C3DVectorWord {
public:                 // C3DAnimationSequenceData::C3DVectorWord, 6 bytes
    short X;                           // +0
    short Y;                           // +2
    short Z;                           // +4
};
};
template C3DAnimationSequenceData::C3DVectorWord* std::__copy_backward<C3DAnimationSequenceData::C3DVectorWord*,C3DAnimationSequenceData::C3DVectorWord*,int >(C3DAnimationSequenceData::C3DVectorWord*, C3DAnimationSequenceData::C3DVectorWord*, C3DAnimationSequenceData::C3DVectorWord*, std::random_access_iterator_tag const&, int*);
