// std::__uninitialized_fill_n(CHeightMapCell* first, unsigned int n, CHeightMapCell const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCHeightMapCell@@IV1@@std@@YIPAVCHeightMapCell@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F6860, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 008f6bb0 std::vector<CHeightMapCell, std::allocator<CHeightMapCell>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAVCHeightMapCell@@IV1@@std@@YIPAVCHeightMapCell@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CHeightMapCell.
#include "stlport/fable_containers.h"
#include "engine/CHeightMapCell.h"
template CHeightMapCell* std::__uninitialized_fill_n<CHeightMapCell*,unsigned int,CHeightMapCell >(CHeightMapCell*, unsigned int, CHeightMapCell const&, std::__false_type const&);
