// std::fill(unsigned __int64* first, unsigned __int64* last, unsigned __int64 const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PA_K_K@std@@YIXPA_K0AB_K@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00613260, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: builtin; caller 00617fd0 source (stdvector_Mfillinsert_00617fd0.cpp) names unsigned __int64; binder ??$fill@PA_K_K@std@@YIXPA_K0AB_K@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over unsigned __int64.
#include "stlport/fable_containers.h"
template void std::fill<unsigned __int64*,unsigned __int64 >(unsigned __int64*, unsigned __int64*, unsigned __int64 const&);
