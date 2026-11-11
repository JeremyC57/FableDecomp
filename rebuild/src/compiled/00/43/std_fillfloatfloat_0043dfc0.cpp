// std::fill(float* first, float* last, float const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAMM@std@@YIXPAM0ABM@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043DFC0, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 00441020 source (stdvectorfloatstdallocatorfloat_Mfillinsert_00441020.cpp) names float; binder ??$fill@PAMM@std@@YIXPAM0ABM@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over float.
#include "stlport/fable_containers.h"
template void std::fill<float*,float >(float*, float*, float const&);
