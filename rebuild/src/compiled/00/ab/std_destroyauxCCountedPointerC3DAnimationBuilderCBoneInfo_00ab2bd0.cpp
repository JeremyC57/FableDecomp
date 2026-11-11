// std::__destroy_aux(CCountedPointer<C3DAnimationBuilder::CBoneInfo>* first, CCountedPointer<C3DAnimationBuilder::CBoneInfo>* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@@std@@YIXPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB2BD0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ab1f30 source (C3DAnimationBuilder_BuildBoneInfoTree_00ab1f30.cpp) names CCountedPointer<C3DAnimationBuilder::CBoneInfo>; caller 00ab3300 source (stdvector_Minsertoverflow_00ab3300.cpp) names CCountedPointer<C3DAnimationBuilder::CBoneInfo>; binder ??$__destroy_aux@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@@std@@YIXPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<C3DAnimationBuilder::CBoneInfo>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace C3DAnimationBuilder {
class CBoneInfo;
}
template void std::__destroy_aux<CCountedPointer<C3DAnimationBuilder::CBoneInfo>* >(CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, std::__false_type const&);
