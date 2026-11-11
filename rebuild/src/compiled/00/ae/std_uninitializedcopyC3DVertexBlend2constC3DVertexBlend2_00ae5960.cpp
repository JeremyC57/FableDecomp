// std::__uninitialized_copy(C3DVertexBlend2 const* first, C3DVertexBlend2 const* last, C3DVertexBlend2* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBVC3DVertexBlend2@@PAV1@@std@@YIPAVC3DVertexBlend2@@PBV1@0PAV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE5960, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00ae6690 std::vector<C3DVertexBlend2, std::allocator<C3DVertexBlend2>>::operator=; binder ??$__uninitialized_copy@PBVC3DVertexBlend2@@PAV1@@std@@YIPAVC3DVertexBlend2@@PBV1@0PAV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DVertexBlend2.
#include "stlport/fable_containers.h"
#include "engine/C3DVertexBlend2.h"
template C3DVertexBlend2* std::__uninitialized_copy<C3DVertexBlend2 const*,C3DVertexBlend2* >(C3DVertexBlend2 const*, C3DVertexBlend2 const*, C3DVertexBlend2*, std::__false_type const&);
