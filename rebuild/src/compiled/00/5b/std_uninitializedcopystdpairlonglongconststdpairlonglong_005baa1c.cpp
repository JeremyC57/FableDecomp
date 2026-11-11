// std::__uninitialized_copy(std::pair<long,long> const* first, std::pair<long,long> const* last, std::pair<long,long>* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PBU?$pair@JJ@std@@PAU12@@std@@YIPAU?$pair@JJ@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BAA1C, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: builtin; caller 005baec4 source (stdvectorstdpairlonglong_vector_005baec4.cpp) names std::pair<long,long>; binder ??$__uninitialized_copy@PBU?$pair@JJ@std@@PAU12@@std@@YIPAU?$pair@JJ@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,long>.
#pragma optimize("s", on)
#include "stlport/fable_containers.h"
template std::pair<long,long>* std::__uninitialized_copy<std::pair<long,long> const*,std::pair<long,long>* >(std::pair<long,long> const*, std::pair<long,long> const*, std::pair<long,long>*, std::__false_type const&);
