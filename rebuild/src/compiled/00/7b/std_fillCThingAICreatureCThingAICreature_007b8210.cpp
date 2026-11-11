// std::fill(CThingAICreature** first, CThingAICreature** last, CThingAICreature* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCThingAICreature@@PAV1@@std@@YIXPAPAVCThingAICreature@@0ABQAV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B8210, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 007b98c0 source (stdvector_Mfillinsert_007b98c0.cpp) names CThingAICreature; binder ??$fill@PAPAVCThingAICreature@@PAV1@@std@@YIXPAPAVCThingAICreature@@0ABQAV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CThingAICreature*.
#include "stlport/fable_containers.h"
class CThingAICreature;
template void std::fill<CThingAICreature**,CThingAICreature* >(CThingAICreature**, CThingAICreature**, CThingAICreature* const&);
