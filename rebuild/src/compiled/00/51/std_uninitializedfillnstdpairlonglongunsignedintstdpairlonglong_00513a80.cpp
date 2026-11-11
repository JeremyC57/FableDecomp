// std::__uninitialized_fill_n(std::pair<long,long>* first, unsigned int n, std::pair<long,long> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@JJ@std@@IU12@@std@@YIPAU?$pair@JJ@0@PAU10@IABU10@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00513A80, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: builtin; caller 0059400c source (stdvector_Minsertoverflow_0059400c.cpp) names std::pair<long,long>; binder ??$__uninitialized_fill_n@PAU?$pair@JJ@std@@IU12@@std@@YIPAU?$pair@JJ@0@PAU10@IABU10@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,long>.
#include "stlport/fable_containers.h"
template std::pair<long,long>* std::__uninitialized_fill_n<std::pair<long,long>*,unsigned int,std::pair<long,long> >(std::pair<long,long>*, unsigned int, std::pair<long,long> const&, std::__false_type const&);
