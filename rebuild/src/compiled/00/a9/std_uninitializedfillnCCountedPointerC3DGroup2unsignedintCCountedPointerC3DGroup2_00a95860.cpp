// std::__uninitialized_fill_n(CCountedPointer<C3DGroup2>* first, unsigned int n, CCountedPointer<C3DGroup2> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VC3DGroup2@@@@IV1@@std@@YIPAV?$CCountedPointer@VC3DGroup2@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A95860, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a97ce0 source (stdvector_Minsertoverflow_00a97ce0.cpp) names CCountedPointer<C3DGroup2>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VC3DGroup2@@@@IV1@@std@@YIPAV?$CCountedPointer@VC3DGroup2@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<C3DGroup2>.
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DGROUP2
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DGroup2.h"
template CCountedPointer<C3DGroup2>* std::__uninitialized_fill_n<CCountedPointer<C3DGroup2>*,unsigned int,CCountedPointer<C3DGroup2> >(CCountedPointer<C3DGroup2>*, unsigned int, CCountedPointer<C3DGroup2> const&, std::__false_type const&);
