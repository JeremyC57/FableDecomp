// std::__uninitialized_copy(C2DCoordI* first, C2DCoordI* last, C2DCoordI* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC2DCoordI@@PAV1@@std@@YIPAVC2DCoordI@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00612830, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00656a40 std::vector<C2DCoordI>::_M_fill_insert; caller 00656a40 std::vector<C2DCoordI>::_M_fill_insert; binder ??$__uninitialized_copy@PAVC2DCoordI@@PAV1@@std@@YIPAVC2DCoordI@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C2DCoordI.
#include "stlport/fable_containers.h"
#include "engine/C2DCoordI.h"
template C2DCoordI* std::__uninitialized_copy<C2DCoordI*,C2DCoordI* >(C2DCoordI*, C2DCoordI*, C2DCoordI*, std::__false_type const&);
