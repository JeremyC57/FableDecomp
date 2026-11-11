// std::fill(std::pair<long,float>* first, std::pair<long,float>* last, std::pair<long,float> const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAU?$pair@JM@std@@U12@@std@@YIXPAU?$pair@JM@0@0ABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00784DE0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 00786020 source (stdvector_Mfillinsert_00786020.cpp) names std::pair<long,float>; binder ??$fill@PAU?$pair@JM@std@@U12@@std@@YIXPAU?$pair@JM@0@0ABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<long,float>.
#include "stlport/fable_containers.h"
template void std::fill<std::pair<long,float>*,std::pair<long,float> >(std::pair<long,float>*, std::pair<long,float>*, std::pair<long,float> const&);
