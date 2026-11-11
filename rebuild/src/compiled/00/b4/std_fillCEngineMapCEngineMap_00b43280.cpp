// std::fill(CEngineMap** first, CEngineMap** last, CEngineMap* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCEngineMap@@PAV1@@std@@YIXPAPAVCEngineMap@@0ABQAV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B43280, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b43f80 source (stdvector_Mfillinsert_00b43f80.cpp) names CEngineMap; binder ??$fill@PAPAVCEngineMap@@PAV1@@std@@YIXPAPAVCEngineMap@@0ABQAV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineMap*.
#include "stlport/fable_containers.h"
class CEngineMap;
template void std::fill<CEngineMap**,CEngineMap* >(CEngineMap**, CEngineMap**, CEngineMap* const&);
