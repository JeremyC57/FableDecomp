// std::uninitialized_fill_n(std::pair<unsigned long,unsigned long>* first, unsigned int n, std::pair<unsigned long,unsigned long> const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAU?$pair@KK@std@@IU12@@std@@YIPAU?$pair@KK@0@PAU10@IABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B2860, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 009b6460 source (stdvector_Mfillinsert_009b6460.cpp) names std::pair<unsigned long,unsigned long>; binder ??$uninitialized_fill_n@PAU?$pair@KK@std@@IU12@@std@@YIPAU?$pair@KK@0@PAU10@IABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned long,unsigned long>.
#include "stlport/fable_containers.h"
template std::pair<unsigned long,unsigned long>* std::uninitialized_fill_n<std::pair<unsigned long,unsigned long>*,unsigned int,std::pair<unsigned long,unsigned long> >(std::pair<unsigned long,unsigned long>*, unsigned int, std::pair<unsigned long,unsigned long> const&);
