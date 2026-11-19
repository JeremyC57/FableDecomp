// std::__adjust_heap(pair<unsigned long, C3DVector>* first, int holeIndex, int len, pair<unsigned long, C3DVector> val, NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> comp) -- STLport heap sift-down for 16-byte (unsigned long key, C3DVector) pairs ordered by an unsigned key less, then __push_heap @ 0x00769470
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@KVC3DVector@@@std@@HU12@V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@std@@YIXPAU?$pair@KVC3DVector@@@0@HHU10@V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769d50, 169 bytes.
// Callee: 0x00769470 __push_heap (same instantiation).  first in ecx, hole in edx (link-time
// __fastcall); bottom, the 16-byte pair by value (built in its argument slot: `sub esp,10h ;
// mov eax,esp` then four dword stores = user copy constructor) and the empty comparator follow
// (`ret 18h`).
// Shape: STLport 4.6 _heap.c `__adjust_heap` with a 16-byte stride (`shl esi,4`); the element
// assignment is four dword moves (implicit operator=), the compare an UNSIGNED `jae` on the key.
// Label: the propagated pair<unsigned long, C3DHelperPoint> is 20 bytes (Ego_r.pdb C3DHelperPoint =
// C3DVector Position + long BoneIndex = 16), which is the 20-stride twin 0x00A32440; this 16-stride
// body carries a 12-byte payload, modelled as a C3DVector.  The argument-slot copy stores four
// dwords straight off eax, so C3DVector has a memberwise user copy constructor (an implicit one
// copies the vector as a block through `add eax,4`).
// Identity (NF96 q11427): Ego_r.pdb 0x007775E0 holds the Dinkumware twin ??$_Adjust_heap@PAU?$pair@KVC3DVector@@@std@@HU12@V?$CParticleUpdateCCompareVector@K@
//   ?$CParticleUpdateParameters@KK@NParticleEngine@@@std@@... over this pair / comparator (retail STL is STLport); the landed
//   __pop_heap_aux 0x0076A590 already binds this STLport spelling. Comparator: NParticleEngine::CParticleUpdateParameters<unsigned long,
//   unsigned long>::CParticleUpdateCCompareVector<unsigned long> (engine/NParticleEngine_CParticleUpdateParameters.h; the CVectorMap
//   VectorParams compare). Descriptive-by-library spelling (in neither PDB as STLport).
// Shared headers: the STLport body of stlport/fable_containers.h (FABLE_RETAIL_STLP_DECAL_HEAP, explicit instantiation; __push_heap stays
//   out of line as retail, the same recipe as the landed 0x0077EA60 / 0x005E1210); was a local Dinkumware-shaped _Adjust_heap over
//   SKeyedVector / SKeyLess models in namespace FableHeap_00769d50.
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
typedef std::pair<unsigned long, C3DVector> TVectorParam;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> TVectorParamCompare;
template void std::__adjust_heap(TVectorParam*, int, int, TVectorParam, TVectorParamCompare);
