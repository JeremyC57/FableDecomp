// std::__copy(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCDegradableInfo@@PAV1@H@std@@YIPAVCDegradableInfo@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067E590, 72 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0067f220 std::vector<CDegradableInfo>::resize; binder ??$__copy@PAVCDegradableInfo@@PAV1@H@std@@YIPAVCDegradableInfo@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CDegradableInfo.
#include "stlport/fable_containers.h"
#include "engine/CDegradableInfo.h"
template CDegradableInfo* std::__copy<CDegradableInfo*,CDegradableInfo*,int >(CDegradableInfo*, CDegradableInfo*, CDegradableInfo*, std::random_access_iterator_tag const&, int*);
