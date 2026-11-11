// std::__uninitialized_copy(C3DGroup2* first, C3DGroup2* last, C3DGroup2* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DGroup2@@PAV1@@std@@YIPAVC3DGroup2@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7480, 54 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00ae80f0 std::vector<C3DGroup2>::reserve; caller 00ae81f0 std::vector<C3DGroup2>::_M_insert_overflow; caller 00ae81f0 std::vector<C3DGroup2>::_M_insert_overflow; binder ??$__uninitialized_copy@PAVC3DGroup2@@PAV1@@std@@YIPAVC3DGroup2@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DGroup2.
#define FABLE_RETAIL_C3DGROUP2
#include "stlport/fable_containers.h"
#include "engine/C3DGroup2.h"
template C3DGroup2* std::__uninitialized_copy<C3DGroup2*,C3DGroup2* >(C3DGroup2*, C3DGroup2*, C3DGroup2*, std::__false_type const&);
