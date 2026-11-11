// std::__uninitialized_copy(CRegion* first, CRegion* last, CRegion* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCRegion@@PAV1@@std@@YIPAVCRegion@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051B420, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0051d0a0 std::vector<CRegion, std::allocator<CRegion>>::_M_fill_insert; caller 0051d0a0 std::vector<CRegion, std::allocator<CRegion>>::_M_fill_insert; callee 0051a900 CRegion::CRegion; binder ??$__uninitialized_copy@PAVCRegion@@PAV1@@std@@YIPAVCRegion@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CRegion.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CREGION
#include "stlport/fable_containers.h"
#include "engine/CRegion.h"
template CRegion* std::__uninitialized_copy<CRegion*,CRegion* >(CRegion*, CRegion*, CRegion*, std::__false_type const&);
