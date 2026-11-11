// std::fill(CQuaternion* first, CQuaternion* last, CQuaternion const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCQuaternion@@V1@@std@@YIXPAVCQuaternion@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A51FE0, 55 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a55190 std::vector<CQuaternion, std::allocator<CQuaternion>>::_M_fill_insert; caller 00a55190 std::vector<CQuaternion, std::allocator<CQuaternion>>::_M_fill_insert; binder ??$fill@PAVCQuaternion@@V1@@std@@YIXPAVCQuaternion@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CQuaternion.
#include "stlport/fable_containers.h"
#include "engine/CQuaternion.h"
template void std::fill<CQuaternion*,CQuaternion >(CQuaternion*, CQuaternion*, CQuaternion const&);
