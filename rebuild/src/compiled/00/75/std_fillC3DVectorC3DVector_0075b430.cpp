// std::fill(C3DVector* first, C3DVector* last, C3DVector const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVC3DVector@@V1@@std@@YIXPAVC3DVector@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075B430, 49 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0075bb00 std::vector<C3DVector, std::allocator<C3DVector>>::_M_fill_insert; caller 0075bb00 std::vector<C3DVector, std::allocator<C3DVector>>::_M_fill_insert; binder ??$fill@PAVC3DVector@@V1@@std@@YIXPAVC3DVector@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DVector.
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
template void std::fill<C3DVector*,C3DVector >(C3DVector*, C3DVector*, C3DVector const&);
