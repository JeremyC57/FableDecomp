// std::__uninitialized_copy(CDefPointer<CCameraModeDef const>* first, CDefPointer<CCameraModeDef const>* last, CDefPointer<CCameraModeDef const>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAV?$CDefPointer@$$CBVCCameraModeDef@@@@PAV1@@std@@YIPAV?$CDefPointer@$$CBVCCameraModeDef@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069BFC0, 45 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0069dbb0 std::vector<CDefPointer<CCameraModeDef const>, std::allocator<CDefPointer<CCameraModeDef const>>>::_M_insert_overflow; binder ??$__uninitialized_copy@PAV?$CDefPointer@$$CBVCCameraModeDef@@@@PAV1@@std@@YIPAV?$CDefPointer@$$CBVCCameraModeDef@@@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CDefPointer<CCameraModeDef const>.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
class CCameraModeDef;
template CDefPointer<CCameraModeDef const>* std::__uninitialized_copy<CDefPointer<CCameraModeDef const>*,CDefPointer<CCameraModeDef const>* >(CDefPointer<CCameraModeDef const>*, CDefPointer<CCameraModeDef const>*, CDefPointer<CCameraModeDef const>*, std::__false_type const&);
