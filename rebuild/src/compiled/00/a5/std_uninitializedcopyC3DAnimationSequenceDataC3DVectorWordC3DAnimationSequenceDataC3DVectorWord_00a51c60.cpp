// std::__uninitialized_copy(C3DAnimationSequenceData::C3DVectorWord* first, C3DAnimationSequenceData::C3DVectorWord* last, C3DAnimationSequenceData::C3DVectorWord* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DVectorWord@C3DAnimationSequenceData@@PAV12@@std@@YIPAVC3DVectorWord@C3DAnimationSequenceData@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A51C60, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a55070 std::vector<C3DAnimationSequenceData::C3DVectorWord, std::allocator<C3DAnimationSequenceData::C3DVectorWord>>::_M_insert_overflow; caller 00a552b0 std::vector<C3DAnimationSequenceData::C3DVectorWord, std::allocator<C3DAnimationSequenceData::C3DVectorWord>>::_M_fill_insert; caller 00a552b0 std::vector<C3DAnimationSequenceData::C3DVectorWord, std::allocator<C3DAnimationSequenceData::C3DVectorWord>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVC3DVectorWord@C3DAnimationSequenceData@@PAV12@@std@@YIPAVC3DVectorWord@C3DAnimationSequenceData@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DAnimationSequenceData::C3DVectorWord.
#include "stlport/fable_containers.h"
class C3DAnimationSequenceData {
public:
class C3DVectorWord {
public:// 6 bytes (Ego_r.pdb)
    unsigned short VertexIndex[3];              // +0x00
};
};
template C3DAnimationSequenceData::C3DVectorWord* std::__uninitialized_copy<C3DAnimationSequenceData::C3DVectorWord*,C3DAnimationSequenceData::C3DVectorWord* >(C3DAnimationSequenceData::C3DVectorWord*, C3DAnimationSequenceData::C3DVectorWord*, C3DAnimationSequenceData::C3DVectorWord*, std::__false_type const&);
