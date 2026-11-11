// std::uninitialized_fill_n(CRegion* first, unsigned int n, CRegion const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCRegion@@IV1@@std@@YIPAVCRegion@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051BA50, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0051d0a0 std::vector<CRegion, std::allocator<CRegion>>::_M_fill_insert; callee 0051a900 CRegion::CRegion; binder ??$uninitialized_fill_n@PAVCRegion@@IV1@@std@@YIPAVCRegion@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CRegion.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CREGION
#include "stlport/fable_containers.h"
#include "engine/CRegion.h"
template CRegion* std::uninitialized_fill_n<CRegion*,unsigned int,CRegion >(CRegion*, unsigned int, CRegion const&);
