// std::__uninitialized_copy(std::pair<long,float>* first, std::pair<long,float>* last, std::pair<long,float>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@JM@std@@PAU12@@std@@YIPAU?$pair@JM@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B5040, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: builtin; caller 00786020 source (stdvector_Mfillinsert_00786020.cpp) names std::pair<long,float>; binder ??$__uninitialized_copy@PAU?$pair@JM@std@@PAU12@@std@@YIPAU?$pair@JM@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,float>.
#include "stlport/fable_containers.h"
template std::pair<long,float>* std::__uninitialized_copy<std::pair<long,float>*,std::pair<long,float>* >(std::pair<long,float>*, std::pair<long,float>*, std::pair<long,float>*, std::__false_type const&);
