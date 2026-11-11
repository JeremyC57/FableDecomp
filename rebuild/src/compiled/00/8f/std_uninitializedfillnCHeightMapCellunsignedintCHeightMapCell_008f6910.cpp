// std::uninitialized_fill_n(CHeightMapCell* first, unsigned int n, CHeightMapCell const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCHeightMapCell@@IV1@@std@@YIPAVCHeightMapCell@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F6910, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 008f6c90 source (stdvector_Mfillinsert_008f6c90.cpp) names CHeightMapCell; binder ??$uninitialized_fill_n@PAVCHeightMapCell@@IV1@@std@@YIPAVCHeightMapCell@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CHeightMapCell.
#include "stlport/fable_containers.h"
#include "engine/CHeightMapCell.h"
template CHeightMapCell* std::uninitialized_fill_n<CHeightMapCell*,unsigned int,CHeightMapCell >(CHeightMapCell*, unsigned int, CHeightMapCell const&);
