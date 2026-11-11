// std::uninitialized_fill_n(CEnvironmentThemeDef* first, unsigned int n, CEnvironmentThemeDef const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCEnvironmentThemeDef@@IV1@@std@@YIPAVCEnvironmentThemeDef@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047725F, 41 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00476ff7 std::vector<CEnvironmentThemeDef, std::allocator<CEnvironmentThemeDef>>::_M_fill_insert; callee 0047318d CEnvironmentThemeDef::CEnvironmentThemeDef; binder ??$uninitialized_fill_n@PAVCEnvironmentThemeDef@@IV1@@std@@YIPAVCEnvironmentThemeDef@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEnvironmentThemeDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CEnvironmentThemeDef.h"
template CEnvironmentThemeDef* std::uninitialized_fill_n<CEnvironmentThemeDef*,unsigned int,CEnvironmentThemeDef >(CEnvironmentThemeDef*, unsigned int, CEnvironmentThemeDef const&);
