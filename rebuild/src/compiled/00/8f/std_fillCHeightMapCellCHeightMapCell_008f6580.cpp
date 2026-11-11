// std::fill(CHeightMapCell* first, CHeightMapCell* last, CHeightMapCell const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCHeightMapCell@@V1@@std@@YIXPAVCHeightMapCell@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F6580, 49 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 008f6c90 source (stdvector_Mfillinsert_008f6c90.cpp) names CHeightMapCell; binder ??$fill@PAVCHeightMapCell@@V1@@std@@YIXPAVCHeightMapCell@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CHeightMapCell.
#include "stlport/fable_containers.h"
#include "engine/CHeightMapCell.h"
template void std::fill<CHeightMapCell*,CHeightMapCell >(CHeightMapCell*, CHeightMapCell*, CHeightMapCell const&);
