// std::uninitialized_fill_n(CMapInfo* first, unsigned int n, CMapInfo const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCMapInfo@@IV1@@std@@YIPAVCMapInfo@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00514DB0, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0051a6d0 std::vector<CMapInfo, std::allocator<CMapInfo>>::_M_fill_insert; callee 005110b0 CMapInfo::CMapInfo; binder ??$uninitialized_fill_n@PAVCMapInfo@@IV1@@std@@YIPAVCMapInfo@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMapInfo.
#include "stlport/fable_containers.h"
#include "engine/CMapInfo.h"
template CMapInfo* std::uninitialized_fill_n<CMapInfo*,unsigned int,CMapInfo >(CMapInfo*, unsigned int, CMapInfo const&);
