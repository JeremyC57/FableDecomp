// std::__uninitialized_copy(C3DIndexedBoneWeight* first, C3DIndexedBoneWeight* last, C3DIndexedBoneWeight* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DIndexedBoneWeight@@PAV1@@std@@YIPAVC3DIndexedBoneWeight@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A95B60, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a97180 source (stdvector_Mfillinsert_00a97180.cpp) names C3DIndexedBoneWeight; binder ??$__uninitialized_copy@PAVC3DIndexedBoneWeight@@PAV1@@std@@YIPAVC3DIndexedBoneWeight@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DIndexedBoneWeight.
#include "stlport/fable_containers.h"
#include "engine/C3DIndexedBoneWeight.h"
template C3DIndexedBoneWeight* std::__uninitialized_copy<C3DIndexedBoneWeight*,C3DIndexedBoneWeight* >(C3DIndexedBoneWeight*, C3DIndexedBoneWeight*, C3DIndexedBoneWeight*, std::__false_type const&);
