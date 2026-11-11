// std::__uninitialized_copy(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DTriangle2@@PAV1@@std@@YIPAVC3DTriangle2@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6E350, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a71550 std::vector<C3DTriangle2, std::allocator<C3DTriangle2>>::_M_insert_overflow; caller 00a96f00 std::vector<C3DTriangle2, std::allocator<C3DTriangle2>>::_M_fill_insert; caller 00a96f00 std::vector<C3DTriangle2, std::allocator<C3DTriangle2>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVC3DTriangle2@@PAV1@@std@@YIPAVC3DTriangle2@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DTriangle2.
#include "stlport/fable_containers.h"
#include "engine/C3DTriangle2.h"
template C3DTriangle2* std::__uninitialized_copy<C3DTriangle2*,C3DTriangle2* >(C3DTriangle2*, C3DTriangle2*, C3DTriangle2*, std::__false_type const&);
