// std::__destroy_aux(CCountedPointer<C3DSkeletalMorphParameter>* first, CCountedPointer<C3DSkeletalMorphParameter>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VC3DSkeletalMorphParameter@@@@@std@@YIXPAV?$CCountedPointer@VC3DSkeletalMorphParameter@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A464E0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a44eb0 source (C3DSkeletalMorph_C3DSkeletalMorph_00a44eb0.cpp) names CCountedPointer<C3DSkeletalMorphParameter>; caller 00a45410 source (C3DSkeletalMorph_Reset_00a45410.cpp) names CCountedPointer<C3DSkeletalMorphParameter>; binder ??$__destroy_aux@PAV?$CCountedPointer@VC3DSkeletalMorphParameter@@@@@std@@YIXPAV?$CCountedPointer@VC3DSkeletalMorphParameter@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<C3DSkeletalMorphParameter>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class C3DSkeletalMorphParameter;
template void std::__destroy_aux<CCountedPointer<C3DSkeletalMorphParameter>* >(CCountedPointer<C3DSkeletalMorphParameter>*, CCountedPointer<C3DSkeletalMorphParameter>*, std::__false_type const&);
