// std::__uninitialized_copy(std::pair<unsigned long,long>* first, std::pair<unsigned long,long>* last, std::pair<unsigned long,long>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@KJ@std@@PAU12@@std@@YIPAU?$pair@KJ@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B25C0, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: builtin; caller 00a39610 source (stdvector_Mrangeinsert_00a39610.cpp) names std::pair<unsigned long,long>; binder ??$__uninitialized_copy@PAU?$pair@KJ@std@@PAU12@@std@@YIPAU?$pair@KJ@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned long,long>.
#include "stlport/fable_containers.h"
template std::pair<unsigned long,long>* std::__uninitialized_copy<std::pair<unsigned long,long>*,std::pair<unsigned long,long>* >(std::pair<unsigned long,long>*, std::pair<unsigned long,long>*, std::pair<unsigned long,long>*, std::__false_type const&);
