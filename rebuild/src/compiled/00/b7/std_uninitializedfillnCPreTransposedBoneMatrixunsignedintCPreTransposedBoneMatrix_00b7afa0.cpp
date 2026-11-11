// std::uninitialized_fill_n(CPreTransposedBoneMatrix* first, unsigned int n, CPreTransposedBoneMatrix const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAUCPreTransposedBoneMatrix@@IU1@@std@@YIPAUCPreTransposedBoneMatrix@@PAU1@IABU1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B7AFA0, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b7cab0 std::vector<CPreTransposedBoneMatrix, std::allocator<CPreTransposedBoneMatrix>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAUCPreTransposedBoneMatrix@@IU1@@std@@YIPAUCPreTransposedBoneMatrix@@PAU1@IABU1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CPreTransposedBoneMatrix.
#include "stlport/fable_containers.h"
#include "engine/CPreTransposedBoneMatrix.h"
template CPreTransposedBoneMatrix* std::uninitialized_fill_n<CPreTransposedBoneMatrix*,unsigned int,CPreTransposedBoneMatrix >(CPreTransposedBoneMatrix*, unsigned int, CPreTransposedBoneMatrix const&);
