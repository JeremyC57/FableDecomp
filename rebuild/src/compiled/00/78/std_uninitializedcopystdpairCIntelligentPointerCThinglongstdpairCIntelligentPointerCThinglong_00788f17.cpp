// std::__uninitialized_copy(std::pair<CIntelligentPointer<CThing>,long>* first, std::pair<CIntelligentPointer<CThing>,long>* last, std::pair<CIntelligentPointer<CThing>,long>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@PAU12@@std@@YIPAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00788F17, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007893ae std::vector<std::pair<CIntelligentPointer<CThing>, long>, std::allocator<std::pair<CIntelligentPointer<CThing>, long>>>::_M_insert_overflow; caller 007893ae std::vector<std::pair<CIntelligentPointer<CThing>, long>, std::allocator<std::pair<CIntelligentPointer<CThing>, long>>>::_M_insert_overflow; callee 00788d88 std::pair<CIntelligentPointer<CThing>, long>::pair; binder ??$__uninitialized_copy@PAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@PAU12@@std@@YIPAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<CIntelligentPointer<CThing>,long>.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
template std::pair<CIntelligentPointer<CThing>,long>* std::__uninitialized_copy<std::pair<CIntelligentPointer<CThing>,long>*,std::pair<CIntelligentPointer<CThing>,long>* >(std::pair<CIntelligentPointer<CThing>,long>*, std::pair<CIntelligentPointer<CThing>,long>*, std::pair<CIntelligentPointer<CThing>,long>*, std::__false_type const&);
