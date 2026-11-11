// std::__copy(CHeightMapCell* first, CHeightMapCell* last, CHeightMapCell* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCHeightMapCell@@PAV1@H@std@@YIPAVCHeightMapCell@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F65C0, 74 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 008f6e30 std::vector<CHeightMapCell, std::allocator<CHeightMapCell>>::resize; binder ??$__copy@PAVCHeightMapCell@@PAV1@H@std@@YIPAVCHeightMapCell@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CHeightMapCell.
#include "stlport/fable_containers.h"
#include "engine/CHeightMapCell.h"
template CHeightMapCell* std::__copy<CHeightMapCell*,CHeightMapCell*,int >(CHeightMapCell*, CHeightMapCell*, CHeightMapCell*, std::random_access_iterator_tag const&, int*);
