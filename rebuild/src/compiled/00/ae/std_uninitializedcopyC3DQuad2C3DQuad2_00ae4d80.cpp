// std::__uninitialized_copy(C3DQuad2* first, C3DQuad2* last, C3DQuad2* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVC3DQuad2@@PAV1@@std@@YIPAVC3DQuad2@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE4D80, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00ae6e60 std::vector<C3DQuad2, std::allocator<C3DQuad2>>::_M_fill_insert; caller 00ae6e60 std::vector<C3DQuad2, std::allocator<C3DQuad2>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVC3DQuad2@@PAV1@@std@@YIPAVC3DQuad2@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DQuad2.
#include "stlport/fable_containers.h"
#include "engine/C3DQuad2.h"
template C3DQuad2* std::__uninitialized_copy<C3DQuad2*,C3DQuad2* >(C3DQuad2*, C3DQuad2*, C3DQuad2*, std::__false_type const&);
