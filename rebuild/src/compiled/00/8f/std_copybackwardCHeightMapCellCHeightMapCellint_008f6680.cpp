// std::__copy_backward(CHeightMapCell* first, CHeightMapCell* last, CHeightMapCell* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVCHeightMapCell@@PAV1@H@std@@YIPAVCHeightMapCell@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F6680, 76 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 008f6c90 source (stdvector_Mfillinsert_008f6c90.cpp) names CHeightMapCell; binder ??$__copy_backward@PAVCHeightMapCell@@PAV1@H@std@@YIPAVCHeightMapCell@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CHeightMapCell.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
#include "engine/CHeightMapCell.h"
template CHeightMapCell* std::__copy_backward<CHeightMapCell*,CHeightMapCell*,int >(CHeightMapCell*, CHeightMapCell*, CHeightMapCell*, std::random_access_iterator_tag const&, int*);
