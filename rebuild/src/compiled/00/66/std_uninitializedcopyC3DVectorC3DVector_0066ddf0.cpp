// std::__uninitialized_copy(C3DVector* first, C3DVector* last, C3DVector* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DVector@@PAV1@@std@@YIPAVC3DVector@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066DDF0, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0075bb00 std::vector<C3DVector, std::allocator<C3DVector>>::_M_fill_insert; caller 0075bb00 std::vector<C3DVector, std::allocator<C3DVector>>::_M_fill_insert; caller 00a70cb0 std::vector<C3DVector, std::allocator<C3DVector>>::_M_range_insert<C3DVector *>; binder ??$__uninitialized_copy@PAVC3DVector@@PAV1@@std@@YIPAVC3DVector@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DVector.
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
template C3DVector* std::__uninitialized_copy<C3DVector*,C3DVector* >(C3DVector*, C3DVector*, C3DVector*, std::__false_type const&);
