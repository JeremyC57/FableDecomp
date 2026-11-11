// std::__uninitialized_fill_n(C2DCoordI* first, unsigned int n, C2DCoordI const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVC2DCoordI@@IV1@@std@@YIPAVC2DCoordI@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00612860, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00656a40 std::vector<C2DCoordI>::_M_fill_insert; binder ??$__uninitialized_fill_n@PAVC2DCoordI@@IV1@@std@@YIPAVC2DCoordI@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C2DCoordI.
#include "stlport/fable_containers.h"
#include "engine/C2DCoordI.h"
template C2DCoordI* std::__uninitialized_fill_n<C2DCoordI*,unsigned int,C2DCoordI >(C2DCoordI*, unsigned int, C2DCoordI const&, std::__false_type const&);
