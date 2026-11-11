// std::fill(long* first, long* last, long const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAJJ@std@@YIXPAJ0ABJ@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043DF80, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 00440f00 source (stdvector_Mfillinsert_00440f00.cpp) names long; binder ??$fill@PAJJ@std@@YIXPAJ0ABJ@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over long.
#include "stlport/fable_containers.h"
template void std::fill<long*,long >(long*, long*, long const&);
