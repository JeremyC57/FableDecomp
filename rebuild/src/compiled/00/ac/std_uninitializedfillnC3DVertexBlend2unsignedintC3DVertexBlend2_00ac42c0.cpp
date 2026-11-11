// std::uninitialized_fill_n(C3DVertexBlend2* first, unsigned int n, C3DVertexBlend2 const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVC3DVertexBlend2@@IV1@@std@@YIPAVC3DVertexBlend2@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC42C0, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ac42f0 source (stdvector_Mfillinsert_00ac42f0.cpp) names C3DVertexBlend2; binder ??$uninitialized_fill_n@PAVC3DVertexBlend2@@IV1@@std@@YIPAVC3DVertexBlend2@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DVertexBlend2.
#include "stlport/fable_containers.h"
#include "engine/C3DVertexBlend2.h"
template C3DVertexBlend2* std::uninitialized_fill_n<C3DVertexBlend2*,unsigned int,C3DVertexBlend2 >(C3DVertexBlend2*, unsigned int, C3DVertexBlend2 const&);
