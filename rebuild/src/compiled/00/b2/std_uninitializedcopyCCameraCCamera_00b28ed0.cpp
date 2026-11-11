// std::__uninitialized_copy(CCamera* first, CCamera* last, CCamera* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCCamera@@PAV1@@std@@YIPAVCCamera@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B28ED0, 131 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00b29840 std::vector<CCamera>::_M_insert_overflow; caller 00b29840 std::vector<CCamera>::_M_insert_overflow; binder ??$__uninitialized_copy@PAVCCamera@@PAV1@@std@@YIPAVCCamera@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCamera.
#include "stlport/fable_containers.h"
#include "engine/CCamera.h"
template CCamera* std::__uninitialized_copy<CCamera*,CCamera* >(CCamera*, CCamera*, CCamera*, std::__false_type const&);
