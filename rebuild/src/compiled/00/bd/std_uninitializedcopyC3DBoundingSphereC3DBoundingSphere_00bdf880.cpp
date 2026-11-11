// std::__uninitialized_copy(C3DBoundingSphere* first, C3DBoundingSphere* last, C3DBoundingSphere* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DBoundingSphere@@PAV1@@std@@YIPAVC3DBoundingSphere@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDF880, 56 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00bdfe70 std::vector<C3DBoundingSphere, std::allocator<C3DBoundingSphere>>::_M_insert_overflow; caller 00bdfe70 std::vector<C3DBoundingSphere, std::allocator<C3DBoundingSphere>>::_M_insert_overflow; caller 00bdff50 std::vector<C3DBoundingSphere, std::allocator<C3DBoundingSphere>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVC3DBoundingSphere@@PAV1@@std@@YIPAVC3DBoundingSphere@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DBoundingSphere.
#include "stlport/fable_containers.h"
#include "engine/C3DBoundingSphere.h"
template C3DBoundingSphere* std::__uninitialized_copy<C3DBoundingSphere*,C3DBoundingSphere* >(C3DBoundingSphere*, C3DBoundingSphere*, C3DBoundingSphere*, std::__false_type const&);
