// std::__uninitialized_copy(CDefPointer<CCreatureGenerationFamilyDef const>* first, CDefPointer<CCreatureGenerationFamilyDef const>* last, CDefPointer<CCreatureGenerationFamilyDef const>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAV?$CDefPointer@$$CBVCCreatureGenerationFamilyDef@@@@PAV1@@std@@YIPAV?$CDefPointer@$$CBVCCreatureGenerationFamilyDef@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C9F70, 45 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007ca250 std::vector<CDefPointer<CCreatureGenerationFamilyDef const>, std::allocator<CDefPointer<CCreatureGenerationFamilyDef const>>>::_M_insert_overflow; binder ??$__uninitialized_copy@PAV?$CDefPointer@$$CBVCCreatureGenerationFamilyDef@@@@PAV1@@std@@YIPAV?$CDefPointer@$$CBVCCreatureGenerationFamilyDef@@@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CDefPointer<CCreatureGenerationFamilyDef const>.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
class CCreatureGenerationFamilyDef;
template CDefPointer<CCreatureGenerationFamilyDef const>* std::__uninitialized_copy<CDefPointer<CCreatureGenerationFamilyDef const>*,CDefPointer<CCreatureGenerationFamilyDef const>* >(CDefPointer<CCreatureGenerationFamilyDef const>*, CDefPointer<CCreatureGenerationFamilyDef const>*, CDefPointer<CCreatureGenerationFamilyDef const>*, std::__false_type const&);
