// std::fill(std::pair<unsigned long,unsigned long>* first, std::pair<unsigned long,unsigned long>* last, std::pair<unsigned long,unsigned long> const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAU?$pair@KK@std@@U12@@std@@YIXPAU?$pair@KK@0@0ABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B15A0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 009b6460 source (stdvector_Mfillinsert_009b6460.cpp) names std::pair<unsigned long,unsigned long>; binder ??$fill@PAU?$pair@KK@std@@U12@@std@@YIXPAU?$pair@KK@0@0ABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned long,unsigned long>.
#include "stlport/fable_containers.h"
template void std::fill<std::pair<unsigned long,unsigned long>*,std::pair<unsigned long,unsigned long> >(std::pair<unsigned long,unsigned long>*, std::pair<unsigned long,unsigned long>*, std::pair<unsigned long,unsigned long> const&);
