// std::__uninitialized_copy(C3DAnimationSequenceData::C3DVectorWord const* first, C3DAnimationSequenceData::C3DVectorWord const* last, C3DAnimationSequenceData::C3DVectorWord* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVC3DVectorWord@C3DAnimationSequenceData@@PAV12@@std@@YIPAVC3DVectorWord@C3DAnimationSequenceData@@PBV12@0PAV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A52040, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a54cb0 std::vector<C3DAnimationSequenceData::C3DVectorWord, std::allocator<C3DAnimationSequenceData::C3DVectorWord>>::operator=; binder ??$__uninitialized_copy@PBVC3DVectorWord@C3DAnimationSequenceData@@PAV12@@std@@YIPAVC3DVectorWord@C3DAnimationSequenceData@@PBV12@0PAV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DAnimationSequenceData::C3DVectorWord.
#include "stlport/fable_containers.h"
class C3DAnimationSequenceData {
public:
class C3DVectorWord {
public:                          // 6 bytes (Ego_r.pdb)
    short X;                                    // +0x00
    short Y;                                    // +0x02
    short Z;                                    // +0x04
};
};
template C3DAnimationSequenceData::C3DVectorWord* std::__uninitialized_copy<C3DAnimationSequenceData::C3DVectorWord const*,C3DAnimationSequenceData::C3DVectorWord* >(C3DAnimationSequenceData::C3DVectorWord const*, C3DAnimationSequenceData::C3DVectorWord const*, C3DAnimationSequenceData::C3DVectorWord*, std::__false_type const&);
