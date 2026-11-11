// std::fill(C3DQuad2* first, C3DQuad2* last, C3DQuad2 const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVC3DQuad2@@V1@@std@@YIXPAVC3DQuad2@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE4F60, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ae6e60 std::vector<C3DQuad2, std::allocator<C3DQuad2>>::_M_fill_insert; binder ??$fill@PAVC3DQuad2@@V1@@std@@YIXPAVC3DQuad2@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DQuad2.
#include "stlport/fable_containers.h"
#include "engine/C3DQuad2.h"
template void std::fill<C3DQuad2*,C3DQuad2 >(C3DQuad2*, C3DQuad2*, C3DQuad2 const&);
