// std::__make_heap(pair<unsigned long, C3DVector>* first, pair<unsigned long, C3DVector>* last, NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> comp, pair<unsigned long, C3DVector>*, int*) -- heapify [first, last) by sifting every parent down, last parent first
// Leaf: ??$__make_heap@PAU?$pair@KVC3DVector@@@std@@V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@U12@H@std@@YIXPAU?$pair@KVC3DVector@@@0@0V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@0PAH@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076A480, 96 bytes, `ret 0Ch`.
// Callers: 0x0076AB59 and 0x0076B544 (__partial_sort 0x0076B530's inlined make_heap).
// Callee: the landed __adjust_heap 0x00769D50 (same instantiation, first in ecx, hole in edx).
// Shape: STLport `__make_heap(first, last, comp, _Tp*, _Distance*)`, /Gr (__fastcall: first in ecx, last in edx;
//   comp and the two null type pointers on the stack): len = last - first (`sar ..,4`), nothing for len < 2,
//   parent = (len - 2) / 2 (signed `cdq ; sub ; sar`), then `__adjust_heap(first, parent, len, _Tp(first[parent]),
//   comp)` down to parent 0. The comparator is re-read from its slot every pass (all registers busy).
// Identity (NF96 q11427): the comparator is the particle vector-parameter map's NParticleEngine::CParticleUpdateParameters<unsigned long,
//   unsigned long>::CParticleUpdateCCompareVector<unsigned long> (Ego_r.pdb 0x007776E0 Dinkumware twin _Make_heap over this pair / comparator;
//   the landed __pop_heap_aux 0x0076A590 binds __adjust_heap 0x00769D50 with it), not the CKeyPairCompareLess<unsigned long, C3DVector> of
//   the old local model; descriptive-by-library STLport spelling. Shared headers: STLport 4.6 _heap.c __make_heap from
//   stlport/fable_containers.h (FABLE_RETAIL_STLP_HEAP + _MAKE_HEAP_BODY, as the landed 0x005E1BE0); was a local std::pair (class key) /
//   CKeyPairCompareLess model.
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_MAKE_HEAP_BODY
#include "stlport/fable_containers.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
typedef std::pair<unsigned long, C3DVector> TVectorParam;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> TVectorParamCompare;
template void FABLE_STLP_FASTCALL std::__make_heap(TVectorParam*, TVectorParam*, TVectorParamCompare, TVectorParam*, int*);
