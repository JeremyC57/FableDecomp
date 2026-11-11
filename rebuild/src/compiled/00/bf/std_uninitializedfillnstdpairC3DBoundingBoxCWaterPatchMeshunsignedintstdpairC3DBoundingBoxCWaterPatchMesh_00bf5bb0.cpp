// std::__uninitialized_fill_n(std::pair<C3DBoundingBox,CWaterPatchMesh*>* first, unsigned int n, std::pair<C3DBoundingBox,CWaterPatchMesh*> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@VC3DBoundingBox@@PAVCWaterPatchMesh@@@std@@IU12@@std@@YIPAU?$pair@VC3DBoundingBox@@PAVCWaterPatchMesh@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF5BB0, 74 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00bf5ee0 std::vector<std::pair<C3DBoundingBox, CWaterPatchMesh *>, std::allocator<std::pair<C3DBoundingBox, CWaterPatchMesh *>>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAU?$pair@VC3DBoundingBox@@PAVCWaterPatchMesh@@@std@@IU12@@std@@YIPAU?$pair@VC3DBoundingBox@@PAVCWaterPatchMesh@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<C3DBoundingBox,CWaterPatchMesh*>.
#include "stlport/fable_containers.h"
#include "engine/C3DBoundingBox.h"
#include "engine/CWaterPatchMesh.h"
template std::pair<C3DBoundingBox,CWaterPatchMesh*>* std::__uninitialized_fill_n<std::pair<C3DBoundingBox,CWaterPatchMesh*>*,unsigned int,std::pair<C3DBoundingBox,CWaterPatchMesh*> >(std::pair<C3DBoundingBox,CWaterPatchMesh*>*, unsigned int, std::pair<C3DBoundingBox,CWaterPatchMesh*> const&, std::__false_type const&);
