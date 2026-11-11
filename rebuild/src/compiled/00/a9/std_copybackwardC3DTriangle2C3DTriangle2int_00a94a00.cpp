// std::__copy_backward(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVC3DTriangle2@@PAV1@H@std@@YIPAVC3DTriangle2@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A94A00, 71 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a96f00 std::vector<C3DTriangle2, std::allocator<C3DTriangle2>>::_M_fill_insert; binder ??$__copy_backward@PAVC3DTriangle2@@PAV1@H@std@@YIPAVC3DTriangle2@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DTriangle2.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
#include "engine/C3DTriangle2.h"
template C3DTriangle2* std::__copy_backward<C3DTriangle2*,C3DTriangle2*,int >(C3DTriangle2*, C3DTriangle2*, C3DTriangle2*, std::random_access_iterator_tag const&, int*);
