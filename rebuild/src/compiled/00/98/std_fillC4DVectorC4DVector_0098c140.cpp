// std::fill(C4DVector* first, C4DVector* last, C4DVector const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVC4DVector@@V1@@std@@YIXPAVC4DVector@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0098C140, 55 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0098d360 source (stdvector_Mfillinsert_0098d360.cpp) names C4DVector; binder ??$fill@PAVC4DVector@@V1@@std@@YIXPAVC4DVector@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C4DVector.
#include "stlport/fable_containers.h"
#include "engine/C4DVector.h"
template void std::fill<C4DVector*,C4DVector >(C4DVector*, C4DVector*, C4DVector const&);
