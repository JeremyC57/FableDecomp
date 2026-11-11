// std::__destroy_aux(CEngineLocalDetailLayerDef* first, CEngineLocalDetailLayerDef* last, std::__false_type const&) -- STLport 4.6 _construct.h: run the destructor of every element of [first, last) (the non-trivial-destructor path)
// Leaf: ??$__destroy_aux@PAVCEngineLocalDetailLayerDef@@@std@@YIXPAVCEngineLocalDetailLayerDef@@0ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00450A90, 28 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 004772a7 std::vector<CEngineLocalDetailLayerDef, std::allocator<CEngineLocalDetailLayerDef>>::operator=; binder ??$__destroy_aux@PAVCEngineLocalDetailLayerDef@@@std@@YIXPAVCEngineLocalDetailLayerDef@@0ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineLocalDetailLayerDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CEngineLocalDetailLayerDef.h"
template void std::__destroy_aux<CEngineLocalDetailLayerDef* >(CEngineLocalDetailLayerDef*, CEngineLocalDetailLayerDef*, std::__false_type const&);
