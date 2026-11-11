// std::fill(CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* first, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* last, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@V12@@std@@YIXPAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC9E10, 55 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00bcc080 std::vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry, std::allocator<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>>::_M_fill_insert; caller 00bcc080 std::vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry, std::allocator<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>>::_M_fill_insert; binder ??$fill@PAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@V12@@std@@YIXPAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry.
#include "stlport/fable_containers.h"
class CEngineInternalPrimitiveSpriteTrail {
public:
class CSpriteTrailEntry {
public: unsigned char _body[0x10];
};
};
template void std::fill<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*,CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry >(CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry const&);
