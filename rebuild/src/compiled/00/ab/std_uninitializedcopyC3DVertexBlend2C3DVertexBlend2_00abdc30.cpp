// std::__uninitialized_copy(C3DVertexBlend2* first, C3DVertexBlend2* last, C3DVertexBlend2* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DVertexBlend2@@PAV1@@std@@YIPAVC3DVertexBlend2@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABDC30, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00ac42f0 source (stdvector_Mfillinsert_00ac42f0.cpp) names C3DVertexBlend2; binder ??$__uninitialized_copy@PAVC3DVertexBlend2@@PAV1@@std@@YIPAVC3DVertexBlend2@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DVertexBlend2.
#include "stlport/fable_containers.h"
#include "engine/C3DVertexBlend2.h"
template C3DVertexBlend2* std::__uninitialized_copy<C3DVertexBlend2*,C3DVertexBlend2* >(C3DVertexBlend2*, C3DVertexBlend2*, C3DVertexBlend2*, std::__false_type const&);
