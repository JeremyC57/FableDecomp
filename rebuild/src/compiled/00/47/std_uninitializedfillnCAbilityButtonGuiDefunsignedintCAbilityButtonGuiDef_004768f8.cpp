// std::uninitialized_fill_n(CAbilityButtonGuiDef* first, unsigned int n, CAbilityButtonGuiDef const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCAbilityButtonGuiDef@@IV1@@std@@YIPAVCAbilityButtonGuiDef@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004768F8, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 004766c2 std::vector<CAbilityButtonGuiDef, std::allocator<CAbilityButtonGuiDef>>::_M_fill_insert; callee 0046f1b4 CAbilityButtonGuiDef::CAbilityButtonGuiDef; binder ??$uninitialized_fill_n@PAVCAbilityButtonGuiDef@@IV1@@std@@YIPAVCAbilityButtonGuiDef@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CAbilityButtonGuiDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CAbilityButtonGuiDef.h"
template CAbilityButtonGuiDef* std::uninitialized_fill_n<CAbilityButtonGuiDef*,unsigned int,CAbilityButtonGuiDef >(CAbilityButtonGuiDef*, unsigned int, CAbilityButtonGuiDef const&);
