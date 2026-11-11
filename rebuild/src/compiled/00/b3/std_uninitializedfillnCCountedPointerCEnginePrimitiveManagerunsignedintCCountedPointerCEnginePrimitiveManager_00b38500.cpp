// std::__uninitialized_fill_n(CCountedPointer<CEnginePrimitiveManager>* first, unsigned int n, CCountedPointer<CEnginePrimitiveManager> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCEnginePrimitiveManager@@@@IV1@@std@@YIPAV?$CCountedPointer@VCEnginePrimitiveManager@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B38500, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00b38bb0 source (stdvector_Minsertoverflow_00b38bb0.cpp) names CCountedPointer<CEnginePrimitiveManager>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCEnginePrimitiveManager@@@@IV1@@std@@YIPAV?$CCountedPointer@VCEnginePrimitiveManager@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CEnginePrimitiveManager>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CEnginePrimitiveManager.h"
template CCountedPointer<CEnginePrimitiveManager>* std::__uninitialized_fill_n<CCountedPointer<CEnginePrimitiveManager>*,unsigned int,CCountedPointer<CEnginePrimitiveManager> >(CCountedPointer<CEnginePrimitiveManager>*, unsigned int, CCountedPointer<CEnginePrimitiveManager> const&, std::__false_type const&);
