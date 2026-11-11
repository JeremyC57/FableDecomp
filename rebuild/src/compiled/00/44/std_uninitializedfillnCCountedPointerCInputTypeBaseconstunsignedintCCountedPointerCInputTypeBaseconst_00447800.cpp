// std::__uninitialized_fill_n(CCountedPointer<CInputTypeBase const>* first, unsigned int n, CCountedPointer<CInputTypeBase const> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@$$CBVCInputTypeBase@@@@IV1@@std@@YIPAV?$CCountedPointer@$$CBVCInputTypeBase@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00447800, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00449050 source (stdvector_Minsertoverflow_00449050.cpp) names CCountedPointer<CInputTypeBase>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@$$CBVCInputTypeBase@@@@IV1@@std@@YIPAV?$CCountedPointer@$$CBVCInputTypeBase@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CInputTypeBase const>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CInputTypeBase;
template CCountedPointer<CInputTypeBase const>* std::__uninitialized_fill_n<CCountedPointer<CInputTypeBase const>*,unsigned int,CCountedPointer<CInputTypeBase const> >(CCountedPointer<CInputTypeBase const>*, unsigned int, CCountedPointer<CInputTypeBase const> const&, std::__false_type const&);
