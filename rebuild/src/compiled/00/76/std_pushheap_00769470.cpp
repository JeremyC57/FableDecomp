// std::__push_heap(pair<unsigned long, C3DVector>* first, int holeIndex, int topIndex, pair<unsigned long, C3DVector> val, NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> comp) -- percolate a keyed 3D vector up a max-heap ordered by the unsigned key
// Flags: /Gr
// Leaf: ??$__push_heap@PAU?$pair@KVC3DVector@@@std@@HU12@V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@std@@YIXPAU?$pair@KVC3DVector@@@0@HHU10@V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@Z
// Retail: Fable.exe @ 0x00769470, 118 bytes (/O2). Called from __adjust_heap @ 0x00769D50 (same instantiation: 16-byte
//   (unsigned long key, C3DVector) pairs, the particle vector-parameter map).
// Shape: STLport `__push_heap` (`hole > top` test, rotated loop) over a 16-byte element (`shl edx,4`); element assignment is the
//   implicit four-dword copy, the key test an UNSIGNED `jae`. /Gr: first=ecx, hole=edx; `ret 18h` = top + 16-byte pair + the
//   empty comparator. Label correction: the propagated pair<long, CRandomAppearanceMorph::CSkeletalMorph> is the SIGNED-key byte
//   twin at 0x0086A7F0.
// Identity (NF96 q11427): Ego_r.pdb 0x00777500 holds the Dinkumware twin ??$_Push_heap@PAU?$pair@KVC3DVector@@@std@@HU12@V?$CParticleUpdate
//   CCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@std@@... over this pair / comparator (retail STL is STLport);
//   descriptive-by-library STLport spelling. Shared headers: the STLport body of stlport/fable_containers.h
//   (FABLE_RETAIL_STLP_DECAL_HEAP, explicit instantiation, the recipe of the landed 0x0077E730); was a local Dinkumware _Push_heap
//   over local C3DVector / pair / CKeyPairCompareLess models in namespace N00769470 (bound by 0x00769D50 as
//   ?_Push_heap@FableHeap_00769d50@@...).
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
typedef std::pair<unsigned long, C3DVector> TVectorParam;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> TVectorParamCompare;
template void std::__push_heap(TVectorParam*, int, int, TVectorParam, TVectorParamCompare);
