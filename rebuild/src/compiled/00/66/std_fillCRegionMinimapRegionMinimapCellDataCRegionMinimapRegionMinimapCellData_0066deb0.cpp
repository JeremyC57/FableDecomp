// std::fill(CRegionMinimap::RegionMinimapCellData* first, CRegionMinimap::RegionMinimapCellData* last, CRegionMinimap::RegionMinimapCellData const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAURegionMinimapCellData@CRegionMinimap@@U12@@std@@YIXPAURegionMinimapCellData@CRegionMinimap@@0ABU12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066DEB0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0066e480 source (stdvector_Mfillinsert_0066e480.cpp) names CRegionMinimap::RegionMinimapCellData; binder ??$fill@PAURegionMinimapCellData@CRegionMinimap@@U12@@std@@YIXPAURegionMinimapCellData@CRegionMinimap@@0ABU12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CRegionMinimap::RegionMinimapCellData.
#include "stlport/fable_containers.h"
class CRegionMinimap {
public:
struct RegionMinimapCellData {
    unsigned char _body[0x8];
};
};
template void std::fill<CRegionMinimap::RegionMinimapCellData*,CRegionMinimap::RegionMinimapCellData >(CRegionMinimap::RegionMinimapCellData*, CRegionMinimap::RegionMinimapCellData*, CRegionMinimap::RegionMinimapCellData const&);
