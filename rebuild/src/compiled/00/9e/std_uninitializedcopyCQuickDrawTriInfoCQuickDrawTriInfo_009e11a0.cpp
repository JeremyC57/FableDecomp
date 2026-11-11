// std::__uninitialized_copy(CQuickDrawTriInfo* first, CQuickDrawTriInfo* last, CQuickDrawTriInfo* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCQuickDrawTriInfo@@PAV1@@std@@YIPAVCQuickDrawTriInfo@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E11A0, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 009e1750 std::vector<CQuickDrawTriInfo>::_M_insert_overflow; callee copy ctor CQuickDrawTriInfo::CQuickDrawTriInfo (catalog)
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CQuickDrawTriInfo.
#include "stlport/fable_containers.h"
#include "engine/CQuickDrawTriInfo.h"
template CQuickDrawTriInfo* std::__uninitialized_copy<CQuickDrawTriInfo*,CQuickDrawTriInfo* >(CQuickDrawTriInfo*, CQuickDrawTriInfo*, CQuickDrawTriInfo*, std::__false_type const&);
