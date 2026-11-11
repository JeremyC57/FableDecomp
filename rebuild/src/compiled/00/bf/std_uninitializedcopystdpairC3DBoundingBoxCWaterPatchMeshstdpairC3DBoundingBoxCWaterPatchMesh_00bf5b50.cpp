// std::__uninitialized_copy(std::pair<C3DBoundingBox,CWaterPatchMesh*>* first, std::pair<C3DBoundingBox,CWaterPatchMesh*>* last, std::pair<C3DBoundingBox,CWaterPatchMesh*>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@VC3DBoundingBox@@PAVCWaterPatchMesh@@@std@@PAU12@@std@@YIPAU?$pair@VC3DBoundingBox@@PAVCWaterPatchMesh@@@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF5B50, 81 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00bf5ee0 std::vector<std::pair<C3DBoundingBox, CWaterPatchMesh *>, std::allocator<std::pair<C3DBoundingBox, CWaterPatchMesh *>>>::_M_insert_overflow; caller 00bf5ee0 std::vector<std::pair<C3DBoundingBox, CWaterPatchMesh *>, std::allocator<std::pair<C3DBoundingBox, CWaterPatchMesh *>>>::_M_insert_overflow; binder ??$__uninitialized_copy@PAU?$pair@VC3DBoundingBox@@PAVCWaterPatchMesh@@@std@@PAU12@@std@@YIPAU?$pair@VC3DBoundingBox@@PAVCWaterPatchMesh@@@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<C3DBoundingBox,CWaterPatchMesh*>.
#include "stlport/fable_containers.h"
#include "engine/C3DBoundingBox.h"
#include "engine/CWaterPatchMesh.h"
template std::pair<C3DBoundingBox,CWaterPatchMesh*>* std::__uninitialized_copy<std::pair<C3DBoundingBox,CWaterPatchMesh*>*,std::pair<C3DBoundingBox,CWaterPatchMesh*>* >(std::pair<C3DBoundingBox,CWaterPatchMesh*>*, std::pair<C3DBoundingBox,CWaterPatchMesh*>*, std::pair<C3DBoundingBox,CWaterPatchMesh*>*, std::__false_type const&);
