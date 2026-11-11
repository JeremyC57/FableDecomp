// std::fill(C3DBoundingSphere* first, C3DBoundingSphere* last, C3DBoundingSphere const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVC3DBoundingSphere@@V1@@std@@YIXPAVC3DBoundingSphere@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDF580, 55 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00bdff50 std::vector<C3DBoundingSphere, std::allocator<C3DBoundingSphere>>::_M_fill_insert; caller 00bdff50 std::vector<C3DBoundingSphere, std::allocator<C3DBoundingSphere>>::_M_fill_insert; binder ??$fill@PAVC3DBoundingSphere@@V1@@std@@YIXPAVC3DBoundingSphere@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DBoundingSphere.
#include "stlport/fable_containers.h"
#include "engine/C3DBoundingSphere.h"
template void std::fill<C3DBoundingSphere*,C3DBoundingSphere >(C3DBoundingSphere*, C3DBoundingSphere*, C3DBoundingSphere const&);
