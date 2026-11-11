// std::uninitialized_fill_n(CLensFlareElementDef* first, unsigned int n, CLensFlareElementDef const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCLensFlareElementDef@@IV1@@std@@YIPAVCLensFlareElementDef@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00476D9D, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00476b67 std::vector<CLensFlareElementDef, std::allocator<CLensFlareElementDef>>::_M_fill_insert; callee 0046f6dc CLensFlareElementDef::CLensFlareElementDef; binder ??$uninitialized_fill_n@PAVCLensFlareElementDef@@IV1@@std@@YIPAVCLensFlareElementDef@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLensFlareElementDef.
#pragma optimize("s",on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "stlport/fable_containers.h"
#include "engine/CLensFlareElementDef.h"
template CLensFlareElementDef* std::uninitialized_fill_n<CLensFlareElementDef*,unsigned int,CLensFlareElementDef >(CLensFlareElementDef*, unsigned int, CLensFlareElementDef const&);
