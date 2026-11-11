// std::__copy_backward(CGameMapCell* first, CGameMapCell* last, CGameMapCell* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVCGameMapCell@@PAV1@H@std@@YIPAVCGameMapCell@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B7A00, 84 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 006b8050 source (stdvector_Mfillinsert_006b8050.cpp) names CGameMapCell; binder ??$__copy_backward@PAVCGameMapCell@@PAV1@H@std@@YIPAVCGameMapCell@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CGameMapCell.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
class CGameMapCell {
public:                     // 7-byte record (stride 7, no alignment padding)
    unsigned char _body[7];
};
template CGameMapCell* std::__copy_backward<CGameMapCell*,CGameMapCell*,int >(CGameMapCell*, CGameMapCell*, CGameMapCell*, std::random_access_iterator_tag const&, int*);
