// std::uninitialized_fill_n(C3DIndexedBoneWeight* first, unsigned int n, C3DIndexedBoneWeight const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVC3DIndexedBoneWeight@@IV1@@std@@YIPAVC3DIndexedBoneWeight@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A95BC0, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a97180 source (stdvector_Mfillinsert_00a97180.cpp) names C3DIndexedBoneWeight; binder ??$uninitialized_fill_n@PAVC3DIndexedBoneWeight@@IV1@@std@@YIPAVC3DIndexedBoneWeight@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DIndexedBoneWeight.
#include "stlport/fable_containers.h"
#include "engine/C3DIndexedBoneWeight.h"
template C3DIndexedBoneWeight* std::uninitialized_fill_n<C3DIndexedBoneWeight*,unsigned int,C3DIndexedBoneWeight >(C3DIndexedBoneWeight*, unsigned int, C3DIndexedBoneWeight const&);
