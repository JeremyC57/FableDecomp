// std::fill(CMapInfo* first, CMapInfo* last, CMapInfo const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCMapInfo@@V1@@std@@YIXPAVCMapInfo@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00515090, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0051a6d0 std::vector<CMapInfo, std::allocator<CMapInfo>>::_M_fill_insert; callee 00513bf0 CMapInfo::operator=; binder ??$fill@PAVCMapInfo@@V1@@std@@YIXPAVCMapInfo@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMapInfo.
#include "stlport/fable_containers.h"
#include "engine/CMapInfo.h"
template void std::fill<CMapInfo*,CMapInfo >(CMapInfo*, CMapInfo*, CMapInfo const&);
