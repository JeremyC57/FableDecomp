// std::__uninitialized_copy(std::pair<long,long>* first, std::pair<long,long>* last, std::pair<long,long>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAU?$pair@JJ@std@@PAU12@@std@@YIPAU?$pair@JJ@0@PAU10@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00592D43, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: builtin; caller 0059400c source (stdvector_Minsertoverflow_0059400c.cpp) names std::pair<long,long>; binder ??$__uninitialized_copy@PAU?$pair@JJ@std@@PAU12@@std@@YIPAU?$pair@JJ@0@PAU10@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,long>.
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
template std::pair<long,long>* std::__uninitialized_copy<std::pair<long,long>*,std::pair<long,long>* >(std::pair<long,long>*, std::pair<long,long>*, std::pair<long,long>*, std::__false_type const&);
