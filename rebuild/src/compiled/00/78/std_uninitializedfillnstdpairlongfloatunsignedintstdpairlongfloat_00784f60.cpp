// std::uninitialized_fill_n(std::pair<long,float>* first, unsigned int n, std::pair<long,float> const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAU?$pair@JM@std@@IU12@@std@@YIPAU?$pair@JM@0@PAU10@IABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00784F60, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 00786020 source (stdvector_Mfillinsert_00786020.cpp) names std::pair<long,float>; binder ??$uninitialized_fill_n@PAU?$pair@JM@std@@IU12@@std@@YIPAU?$pair@JM@0@PAU10@IABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,float>.
#include "stlport/fable_containers.h"
template std::pair<long,float>* std::uninitialized_fill_n<std::pair<long,float>*,unsigned int,std::pair<long,float> >(std::pair<long,float>*, unsigned int, std::pair<long,float> const&);
