// std::__uninitialized_fill_n(CCountedPointer<C3DSkeletalMorphParameter>* first, unsigned int n, CCountedPointer<C3DSkeletalMorphParameter> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VC3DSkeletalMorphParameter@@@@IV1@@std@@YIPAV?$CCountedPointer@VC3DSkeletalMorphParameter@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A45EA0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a46cb0 source (stdvector_Minsertoverflow_00a46cb0.cpp) names CCountedPointer<C3DSkeletalMorphParameter>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VC3DSkeletalMorphParameter@@@@IV1@@std@@YIPAV?$CCountedPointer@VC3DSkeletalMorphParameter@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<C3DSkeletalMorphParameter>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class C3DSkeletalMorphParameter;
template CCountedPointer<C3DSkeletalMorphParameter>* std::__uninitialized_fill_n<CCountedPointer<C3DSkeletalMorphParameter>*,unsigned int,CCountedPointer<C3DSkeletalMorphParameter> >(CCountedPointer<C3DSkeletalMorphParameter>*, unsigned int, CCountedPointer<C3DSkeletalMorphParameter> const&, std::__false_type const&);
