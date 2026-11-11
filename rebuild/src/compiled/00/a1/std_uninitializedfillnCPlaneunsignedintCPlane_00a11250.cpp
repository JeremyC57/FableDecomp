// std::__uninitialized_fill_n(CPlane* first, unsigned int n, CPlane const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCPlane@@IV1@@std@@YIPAVCPlane@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A11250, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a13720 std::vector<CPlane, std::allocator<CPlane>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAVCPlane@@IV1@@std@@YIPAVCPlane@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CPlane.
#include "stlport/fable_containers.h"
#include "engine/CPlane.h"
template CPlane* std::__uninitialized_fill_n<CPlane*,unsigned int,CPlane >(CPlane*, unsigned int, CPlane const&, std::__false_type const&);
