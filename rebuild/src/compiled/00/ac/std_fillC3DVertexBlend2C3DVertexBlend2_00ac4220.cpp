// std::fill(C3DVertexBlend2* first, C3DVertexBlend2* last, C3DVertexBlend2 const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVC3DVertexBlend2@@V1@@std@@YIXPAVC3DVertexBlend2@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC4220, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ac42f0 source (stdvector_Mfillinsert_00ac42f0.cpp) names C3DVertexBlend2; binder ??$fill@PAVC3DVertexBlend2@@V1@@std@@YIXPAVC3DVertexBlend2@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DVertexBlend2.
#include "stlport/fable_containers.h"
#include "engine/C3DVertexBlend2.h"
template void std::fill<C3DVertexBlend2*,C3DVertexBlend2 >(C3DVertexBlend2*, C3DVertexBlend2*, C3DVertexBlend2 const&);
