// std::__uninitialized_fill_n(CCountedPointer<C3DAnimationBuilder::CBoneInfo>* first, unsigned int n, CCountedPointer<C3DAnimationBuilder::CBoneInfo> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@IV1@@std@@YIPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB1000, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00ab3300 source (stdvector_Minsertoverflow_00ab3300.cpp) names CCountedPointer<C3DAnimationBuilder::CBoneInfo>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@IV1@@std@@YIPAV?$CCountedPointer@VCBoneInfo@C3DAnimationBuilder@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<C3DAnimationBuilder::CBoneInfo>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
namespace C3DAnimationBuilder {
class CBoneInfo;
}
template CCountedPointer<C3DAnimationBuilder::CBoneInfo>* std::__uninitialized_fill_n<CCountedPointer<C3DAnimationBuilder::CBoneInfo>*,unsigned int,CCountedPointer<C3DAnimationBuilder::CBoneInfo> >(CCountedPointer<C3DAnimationBuilder::CBoneInfo>*, unsigned int, CCountedPointer<C3DAnimationBuilder::CBoneInfo> const&, std::__false_type const&);
