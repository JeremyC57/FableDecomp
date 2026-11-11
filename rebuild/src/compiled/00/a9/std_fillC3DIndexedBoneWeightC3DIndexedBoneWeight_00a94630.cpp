// std::fill(C3DIndexedBoneWeight* first, C3DIndexedBoneWeight* last, C3DIndexedBoneWeight const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVC3DIndexedBoneWeight@@V1@@std@@YIXPAVC3DIndexedBoneWeight@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A94630, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a97180 source (stdvector_Mfillinsert_00a97180.cpp) names C3DIndexedBoneWeight; binder ??$fill@PAVC3DIndexedBoneWeight@@V1@@std@@YIXPAVC3DIndexedBoneWeight@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DIndexedBoneWeight.
#include "stlport/fable_containers.h"
#include "engine/C3DIndexedBoneWeight.h"
template void std::fill<C3DIndexedBoneWeight*,C3DIndexedBoneWeight >(C3DIndexedBoneWeight*, C3DIndexedBoneWeight*, C3DIndexedBoneWeight const&);
