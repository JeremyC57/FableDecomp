// std::__uninitialized_copy(CDefPointer<CTattooDef const>* first, CDefPointer<CTattooDef const>* last, CDefPointer<CTattooDef const>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAV?$CDefPointer@$$CBVCTattooDef@@@@PAV1@@std@@YIPAV?$CDefPointer@$$CBVCTattooDef@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071F1F0, 45 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00721b10 std::vector<CDefPointer<CTattooDef const>, std::allocator<CDefPointer<CTattooDef const>>>::_M_insert_overflow; binder ??$__uninitialized_copy@PAV?$CDefPointer@$$CBVCTattooDef@@@@PAV1@@std@@YIPAV?$CDefPointer@$$CBVCTattooDef@@@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CDefPointer<CTattooDef const>.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
#include "engine/CTattooDef.h"
template CDefPointer<CTattooDef const>* std::__uninitialized_copy<CDefPointer<CTattooDef const>*,CDefPointer<CTattooDef const>* >(CDefPointer<CTattooDef const>*, CDefPointer<CTattooDef const>*, CDefPointer<CTattooDef const>*, std::__false_type const&);
