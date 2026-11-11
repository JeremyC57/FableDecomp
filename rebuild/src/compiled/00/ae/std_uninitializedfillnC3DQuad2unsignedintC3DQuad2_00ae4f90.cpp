// std::uninitialized_fill_n(C3DQuad2* first, unsigned int n, C3DQuad2 const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVC3DQuad2@@IV1@@std@@YIPAVC3DQuad2@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE4F90, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ae6e60 std::vector<C3DQuad2, std::allocator<C3DQuad2>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVC3DQuad2@@IV1@@std@@YIPAVC3DQuad2@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DQuad2.
#include "stlport/fable_containers.h"
#include "engine/C3DQuad2.h"
template C3DQuad2* std::uninitialized_fill_n<C3DQuad2*,unsigned int,C3DQuad2 >(C3DQuad2*, unsigned int, C3DQuad2 const&);
