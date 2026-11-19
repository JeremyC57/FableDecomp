// std::__partial_sort(pair<unsigned long, C3DVector>* first, pair<unsigned long, C3DVector>* middle, pair<unsigned long, C3DVector>* last, pair<unsigned long, C3DVector>*, NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> comp) -- heap-select the smallest keys into [first, middle) and sort them
// Leaf: ??$__partial_sort@PAU?$pair@KVC3DVector@@@std@@U12@V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@std@@YIXPAU?$pair@KVC3DVector@@@0@000V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B530, 153 bytes (+int3 pad to 160), `ret 0Ch`.
// Callers: 0x0076BB5C and 0x0076BEF6 (the introsort loop 0x0076BE30's depth-limit fallback).
// Callees: __make_heap 0x0076A480, __adjust_heap 0x00769D50 (the inlined __pop_heap), sort_heap 0x0076AF10 (all the
//   same instantiation, /Gr __fastcall).
// Shape: STLport `__partial_sort(first, middle, last, _Tp*, comp)`: the inline make_heap forwards to __make_heap
//   with two null type pointers (`push 0 ; push 0 ; push comp`); for every i in [middle, last) with
//   comp(*i, *first) the inlined `__pop_heap(first, middle, i, _Tp(*i), comp, _Distance*)` stores *first into *i
//   and sifts the saved value down from the root (`xor edx,edx`); VC holds the saved vector's X/Y on the x87 stack
//   across the `*i = *first` copy. The trailing sort_heap is an out-of-line call.
// Identity (NF96 q11427): the comparator is the particle vector-parameter map's NParticleEngine::CParticleUpdateParameters<unsigned long,
//   unsigned long>::CParticleUpdateCCompareVector<unsigned long> (Ego_r.pdb Dinkumware twins _Make_heap 0x007776E0 / _Sort_heap 0x007784F0
//   over this pair / comparator; the landed introsort loop 0x0076BE30 binds this row with it), not the CKeyPairCompareLess<unsigned long,
//   C3DVector> of the old local model; descriptive-by-library STLport spelling. Shared headers: STLport 4.6 _algo.c __partial_sort from
//   stlport/fable_containers.h (FABLE_RETAIL_STLP_PARTIAL_SORT_BODY + _INLINE_MAKE_HEAP + FABLE_RETAIL_STLP_HEAP, as the landed 0x005E2EB0:
//   __pop_heap's body visible and expanded); was a local std::pair (class key) / CKeyPairCompareLess model.
#define FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
typedef std::pair<unsigned long, C3DVector> TVectorParam;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> TVectorParamCompare;
template void std::__partial_sort(TVectorParam*, TVectorParam*, TVectorParam*, TVectorParam*, TVectorParamCompare);
