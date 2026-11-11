// std::__uninitialized_copy(C2DLineF* first, C2DLineF* last, C2DLineF* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC2DLineF@@PAV1@@std@@YIPAVC2DLineF@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A13490, 56 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a13800 std::vector<C2DLineF, std::allocator<C2DLineF>>::_M_insert_overflow; caller 00a13800 std::vector<C2DLineF, std::allocator<C2DLineF>>::_M_insert_overflow; caller 00a74ac0 std::vector<C2DLineF, std::allocator<C2DLineF>>::_M_range_insert<C2DLineF *>; binder ??$__uninitialized_copy@PAVC2DLineF@@PAV1@@std@@YIPAVC2DLineF@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C2DLineF.
#include "stlport/fable_containers.h"
#include "engine/C2DLineF.h"
template C2DLineF* std::__uninitialized_copy<C2DLineF*,C2DLineF* >(C2DLineF*, C2DLineF*, C2DLineF*, std::__false_type const&);
