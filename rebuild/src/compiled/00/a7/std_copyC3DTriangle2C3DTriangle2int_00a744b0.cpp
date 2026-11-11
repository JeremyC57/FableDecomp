// std::__copy(C3DTriangle2* first, C3DTriangle2* last, C3DTriangle2* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVC3DTriangle2@@PAV1@H@std@@YIPAVC3DTriangle2@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A744B0, 70 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a97a50 std::vector<C3DTriangle2>::resize; binder ??$__copy@PAVC3DTriangle2@@PAV1@H@std@@YIPAVC3DTriangle2@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DTriangle2.
#include "stlport/fable_containers.h"
#include "engine/C3DTriangle2.h"
template C3DTriangle2* std::__copy<C3DTriangle2*,C3DTriangle2*,int >(C3DTriangle2*, C3DTriangle2*, C3DTriangle2*, std::random_access_iterator_tag const&, int*);
