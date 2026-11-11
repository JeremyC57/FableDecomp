// std::__copy_backward(C3DVector* first, C3DVector* last, C3DVector* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVC3DVector@@PAV1@H@std@@YIPAVC3DVector@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075B5B0, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0075bb00 std::vector<C3DVector, std::allocator<C3DVector>>::_M_fill_insert; caller 007653b0 std::vector<C3DVector, std::allocator<C3DVector>>::insert; caller 00a70cb0 std::vector<C3DVector, std::allocator<C3DVector>>::_M_range_insert<C3DVector *>; binder ??$__copy_backward@PAVC3DVector@@PAV1@H@std@@YIPAVC3DVector@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DVector.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
template C3DVector* std::__copy_backward<C3DVector*,C3DVector*,int >(C3DVector*, C3DVector*, C3DVector*, std::random_access_iterator_tag const&, int*);
