// std::fill(C3DVertex2* first, C3DVertex2* last, C3DVertex2 const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVC3DVertex2@@V1@@std@@YIXPAVC3DVertex2@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A94600, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a97040 std::vector<C3DVertex2, std::allocator<C3DVertex2>>::_M_fill_insert; caller 00a97040 std::vector<C3DVertex2, std::allocator<C3DVertex2>>::_M_fill_insert; binder ??$fill@PAVC3DVertex2@@V1@@std@@YIXPAVC3DVertex2@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DVertex2.
#include "stlport/fable_containers.h"
#include "engine/C3DVertex2.h"
template void std::fill<C3DVertex2*,C3DVertex2 >(C3DVertex2*, C3DVertex2*, C3DVertex2 const&);
