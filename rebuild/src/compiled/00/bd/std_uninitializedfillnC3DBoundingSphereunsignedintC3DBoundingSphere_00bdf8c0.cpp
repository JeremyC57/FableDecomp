// std::__uninitialized_fill_n(C3DBoundingSphere* first, unsigned int n, C3DBoundingSphere const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVC3DBoundingSphere@@IV1@@std@@YIPAVC3DBoundingSphere@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDF8C0, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00bdfe70 std::vector<C3DBoundingSphere, std::allocator<C3DBoundingSphere>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAVC3DBoundingSphere@@IV1@@std@@YIPAVC3DBoundingSphere@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DBoundingSphere.
#include "stlport/fable_containers.h"
#include "engine/C3DBoundingSphere.h"
template C3DBoundingSphere* std::__uninitialized_fill_n<C3DBoundingSphere*,unsigned int,C3DBoundingSphere >(C3DBoundingSphere*, unsigned int, C3DBoundingSphere const&, std::__false_type const&);
