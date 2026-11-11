// std::uninitialized_fill_n(CFeatDef* first, unsigned int n, CFeatDef const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCFeatDef@@IV1@@std@@YIPAVCFeatDef@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F34830, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00f34660 std::vector<CFeatDef, std::allocator<CFeatDef>>::_M_fill_insert; callee 00f2d270 CFeatDef::CFeatDef; binder ??$uninitialized_fill_n@PAVCFeatDef@@IV1@@std@@YIPAVCFeatDef@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CFeatDef.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "stlport/fable_containers.h"
#include "engine/CFeatDef.h"
template CFeatDef* std::uninitialized_fill_n<CFeatDef*,unsigned int,CFeatDef >(CFeatDef*, unsigned int, CFeatDef const&);
