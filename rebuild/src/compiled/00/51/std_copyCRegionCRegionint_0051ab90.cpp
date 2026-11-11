// std::__copy(CRegion* first, CRegion* last, CRegion* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCRegion@@PAV1@H@std@@YIPAVCRegion@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051AB90, 67 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0051cc80 std::vector<CRegion, std::allocator<CRegion>>::erase; callee 00519b80 CRegion::operator=; binder ??$__copy@PAVCRegion@@PAV1@H@std@@YIPAVCRegion@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CRegion.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CREGION
#include "stlport/fable_containers.h"
#include "engine/CRegion.h"
template CRegion* std::__copy<CRegion*,CRegion*,int >(CRegion*, CRegion*, CRegion*, std::random_access_iterator_tag const&, int*);
