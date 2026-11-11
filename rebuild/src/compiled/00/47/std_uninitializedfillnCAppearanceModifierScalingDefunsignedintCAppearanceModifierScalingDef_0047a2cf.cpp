// std::uninitialized_fill_n(CAppearanceModifierScalingDef* first, unsigned int n, CAppearanceModifierScalingDef const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCAppearanceModifierScalingDef@@IV1@@std@@YIPAVCAppearanceModifierScalingDef@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047A2CF, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: callee 004784b3 CAppearanceModifierScalingDef::CAppearanceModifierScalingDef; binder ??$uninitialized_fill_n@PAVCAppearanceModifierScalingDef@@IV1@@std@@YIPAVCAppearanceModifierScalingDef@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CAppearanceModifierScalingDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CAppearanceModifierScalingDef.h"
template CAppearanceModifierScalingDef* std::uninitialized_fill_n<CAppearanceModifierScalingDef*,unsigned int,CAppearanceModifierScalingDef >(CAppearanceModifierScalingDef*, unsigned int, CAppearanceModifierScalingDef const&);
