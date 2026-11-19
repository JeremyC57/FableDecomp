// std::__find(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, const CIntelligentPointer<CThing>& val, const random_access_iterator_tag&) -- STLport random-access find (4-way unrolled), equality through GetPItem
// Leaf: ??$__find@PAV?$CIntelligentPointer@VCThing@@@@V1@@std@@YIPAV?$CIntelligentPointer@VCThing@@@@PAV1@0ABV1@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C4060, 249 bytes, __fastcall (first ecx, last edx; &val and the tag pushed).
// Layout: CIntelligentPointer<CThing> stride 8, CBaseIntelligentPointer::GetPItem 0x00A01B50 reads pointer+4; SGI random-access find unroll 4.
// Name (NF94 q11421): STLport 4.6.2 _algobase.c __find (random-access overload), the decorated name the landed caller 0x008DD7E0 binds
//   (reloc_bindings); descriptive-by-library (Ego_r / FableWin are Dinkumware). Single source for the spelling beyond the binding.
// Shared headers: stlport/fable_algorithm.h's original _algobase.c body (FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH with the pass-21
//   FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST text, which also carries CIntelligentPointer's same-type operator==); was a local
//   FableFind_006c4060 namespace model (CBaseIntelligentPointer, random_access_iterator_tag, find).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CIntelligentPointer.h"
class CThing;
template CIntelligentPointer<CThing>* FABLE_FASTCALL std::__find(CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>*,
                                                                 const CIntelligentPointer<CThing>&, const std::random_access_iterator_tag&);
