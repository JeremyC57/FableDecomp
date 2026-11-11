// std::fill(CPreTransposedBoneMatrix* first, CPreTransposedBoneMatrix* last, CPreTransposedBoneMatrix const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAUCPreTransposedBoneMatrix@@U1@@std@@YIXPAUCPreTransposedBoneMatrix@@0ABU1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B7AAC0, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b7cab0 std::vector<CPreTransposedBoneMatrix, std::allocator<CPreTransposedBoneMatrix>>::_M_fill_insert; caller 00b7cab0 std::vector<CPreTransposedBoneMatrix, std::allocator<CPreTransposedBoneMatrix>>::_M_fill_insert; binder ??$fill@PAUCPreTransposedBoneMatrix@@U1@@std@@YIXPAUCPreTransposedBoneMatrix@@0ABU1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CPreTransposedBoneMatrix.
#include "stlport/fable_containers.h"
#include "engine/CPreTransposedBoneMatrix.h"
template void std::fill<CPreTransposedBoneMatrix*,CPreTransposedBoneMatrix >(CPreTransposedBoneMatrix*, CPreTransposedBoneMatrix*, CPreTransposedBoneMatrix const&);
