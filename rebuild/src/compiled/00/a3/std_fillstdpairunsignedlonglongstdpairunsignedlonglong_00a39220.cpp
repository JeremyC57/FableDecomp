// std::fill(std::pair<unsigned long,long>* first, std::pair<unsigned long,long>* last, std::pair<unsigned long,long> const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAU?$pair@KJ@std@@U12@@std@@YIXPAU?$pair@KJ@0@0ABU10@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A39220, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 00a397d0 source (stdvector_Mfillinsert_00a397d0.cpp) names std::pair<unsigned long,long>; binder ??$fill@PAU?$pair@KJ@std@@U12@@std@@YIXPAU?$pair@KJ@0@0ABU10@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over std::pair<unsigned long,long>.
#include "stlport/fable_containers.h"
template void std::fill<std::pair<unsigned long,long>*,std::pair<unsigned long,long> >(std::pair<unsigned long,long>*, std::pair<unsigned long,long>*, std::pair<unsigned long,long> const&);
