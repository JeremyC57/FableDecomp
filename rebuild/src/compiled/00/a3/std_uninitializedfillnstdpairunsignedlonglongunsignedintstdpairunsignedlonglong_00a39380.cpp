// std::uninitialized_fill_n(std::pair<unsigned long,long>* first, unsigned int n, std::pair<unsigned long,long> const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAU?$pair@KJ@std@@IU12@@std@@YIPAU?$pair@KJ@0@PAU10@IABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A39380, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 00a397d0 source (stdvector_Mfillinsert_00a397d0.cpp) names std::pair<unsigned long,long>; binder ??$uninitialized_fill_n@PAU?$pair@KJ@std@@IU12@@std@@YIPAU?$pair@KJ@0@PAU10@IABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned long,long>.
#include "stlport/fable_containers.h"
template std::pair<unsigned long,long>* std::uninitialized_fill_n<std::pair<unsigned long,long>*,unsigned int,std::pair<unsigned long,long> >(std::pair<unsigned long,long>*, unsigned int, std::pair<unsigned long,long> const&);
