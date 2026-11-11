// std::__uninitialized_fill_n(C2DLineF* first, unsigned int n, C2DLineF const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVC2DLineF@@IV1@@std@@YIPAVC2DLineF@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A134D0, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a13800 std::vector<C2DLineF, std::allocator<C2DLineF>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAVC2DLineF@@IV1@@std@@YIPAVC2DLineF@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C2DLineF.
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
template C2DLineF* std::__uninitialized_fill_n<C2DLineF*,unsigned int,C2DLineF >(C2DLineF*, unsigned int, C2DLineF const&, std::__false_type const&);
