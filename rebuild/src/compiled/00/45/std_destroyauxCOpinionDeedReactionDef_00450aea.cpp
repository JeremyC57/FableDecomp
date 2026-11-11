// std::__destroy_aux(COpinionDeedReactionDef* first, COpinionDeedReactionDef* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAVCOpinionDeedReactionDef@@@std@@YIXPAVCOpinionDeedReactionDef@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00450AEA, 28 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00471cc5 std::vector<COpinionDeedReactionDef>::operator=; caller 00471df4 std::vector<COpinionDeedReactionDef>::_M_clear; binder ??$__destroy_aux@PAVCOpinionDeedReactionDef@@@std@@YIXPAVCOpinionDeedReactionDef@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over COpinionDeedReactionDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_containers.h"
#include "engine/COpinionDeedReactionDef.h"
template void std::__destroy_aux<COpinionDeedReactionDef* >(COpinionDeedReactionDef*, COpinionDeedReactionDef*, std::__false_type const&);
