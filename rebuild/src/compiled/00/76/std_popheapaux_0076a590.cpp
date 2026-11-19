// std::__pop_heap_aux(pair<unsigned long, C3DVector>* first, pair<unsigned long, C3DVector>* last, pair<unsigned long, C3DVector>*, CParticleUpdateCCompareVector<unsigned long> comp) -- STLport pop_heap front end for the particle vector-parameter map: move the back element out, put the root there and sift the old back element down
// Leaf: ??$__pop_heap_aux@PAU?$pair@KVC3DVector@@@std@@U12@V?$CParticleUpdateCCompareVector@K@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076A590, 97 bytes, /Gr `__fastcall` (first ecx, last edx; the unused T* and the empty
//   comparator on the stack), `ret 8`.
// Identity: the element is 16 bytes (`sar edx,4`); its callee 0x00769D50 is bound (reloc_bindings) to the STLport
//   __adjust_heap<pair<unsigned long,C3DVector>*,...> of the landed make_heap 0x0076A480. The comparator is the PDB's
//   NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>::CParticleUpdateCCompareVector<unsigned long> (Ego_r
//   tc_object_augmentations.obj heap instantiations over pair<unsigned long,C3DVector> all use it; the landed twin 0x0076AC70 is the
//   CParticleUpdateCCompareParamDesc instance of the same TU). The catalog label `_Pop_heap_0<pair<long,EHeroAbility>*...>` is propagated.
// Shape: shared STLport 4.6 _heap.c __pop_heap_aux (FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22) with __pop_heap's body visible
//   (FABLE_RETAIL_STLP_HEAP) and __adjust_heap out of line.
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_POP_HEAP_AUX_PASS22
#include "stlport/fable_containers.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"

typedef std::pair<unsigned long, C3DVector> TVectorParam;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareVector<unsigned long> TVectorParamCompare;
template void FABLE_STLP_FASTCALL std::__pop_heap_aux<TVectorParam*, TVectorParam, TVectorParamCompare>(TVectorParam*, TVectorParam*, TVectorParam*, TVectorParamCompare);