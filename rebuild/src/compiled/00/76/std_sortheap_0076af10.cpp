// std::sort_heap(pair<unsigned long, C3DVector>* first, pair<unsigned long, C3DVector>* last, NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> comp) -- STLport sort_heap over 16-byte (unsigned long key, C3DVector) pairs: pop the root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@KVC3DVector@@@std@@V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@std@@YIXPAU?$pair@KVC3DVector@@@0@0V?$CParticleUpdateCCompareVector@K@?$CParticleUpdateParameters@KK@NParticleEngine@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076AF10, length 144 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned __int64,CANavQuadTreeNode*>*,...>` is propagated over the
// byte-identical skeleton. The only callee is the STLport `__adjust_heap` @ 0x00769D50 for 16-byte
// (unsigned long key, C3DVector) pairs with an unsigned key less; the element model is copied from it.
// Callees: __adjust_heap @ 0x00769D50 (out of line, __fastcall: first in ecx, hole 0 in edx, `ret 18h`).
// Shape: /Gr STLport TU: `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap / __pop_heap_aux / __pop_heap inline
// (16-byte stride: the count test is `and eax,-16 ; cmp eax,16`). The `_Ty(*(_Last - 1))` temporary stays in
// registers (key in ecx, X/Y on the x87 stack, Z spilled to the frame) because both copy constructors are
// memberwise user constructors; `*_Dest = *_First` is the implicit assignment (four dword moves) and the value
// argument is stored field by field from the temporary.
// Identity (NF96 q11427): Ego_r.pdb 0x007784F0 / FableWin.pdb 0x02600120 hold the Dinkumware _Sort_heap / sort_heap over this pair and the
//   particle vector-parameter comparator NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector
//   <unsigned long> (retail STL is STLport); only caller __partial_sort 0x0076B530. Descriptive-by-library STLport spelling.
// Shared headers: STLport 4.6 _heap.h / _heap.c text of stlport/fable_containers.h (FABLE_RETAIL_STLP_SORT_HEAP_BODY, the recipe of the landed
//   0x0077F190 family); was a local Dinkumware _Sort_heap over SKeyedVector / SKeyLess models.
#define FABLE_RETAIL_STLP_SORT_HEAP_BODY
#include "stlport/fable_containers.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
typedef std::pair<unsigned long, C3DVector> TVectorParam;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> TVectorParamCompare;
template void std::sort_heap(TVectorParam*, TVectorParam*, TVectorParamCompare);
