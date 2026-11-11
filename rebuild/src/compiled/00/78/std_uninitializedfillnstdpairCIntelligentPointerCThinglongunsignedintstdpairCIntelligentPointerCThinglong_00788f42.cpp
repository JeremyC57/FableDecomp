// std::__uninitialized_fill_n(std::pair<CIntelligentPointer<CThing>,long>* first, unsigned int n, std::pair<CIntelligentPointer<CThing>,long> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@IU12@@std@@YIPAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00788F42, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007893ae std::vector<std::pair<CIntelligentPointer<CThing>, long>, std::allocator<std::pair<CIntelligentPointer<CThing>, long>>>::_M_insert_overflow; callee 00788d88 std::pair<CIntelligentPointer<CThing>, long>::pair; binder ??$__uninitialized_fill_n@PAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@std@@IU12@@std@@YIPAU?$pair@V?$CIntelligentPointer@VCThing@@@@J@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<CIntelligentPointer<CThing>,long>.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
template std::pair<CIntelligentPointer<CThing>,long>* std::__uninitialized_fill_n<std::pair<CIntelligentPointer<CThing>,long>*,unsigned int,std::pair<CIntelligentPointer<CThing>,long> >(std::pair<CIntelligentPointer<CThing>,long>*, unsigned int, std::pair<CIntelligentPointer<CThing>,long> const&, std::__false_type const&);
