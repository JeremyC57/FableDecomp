// std::uninitialized_fill_n(CRegionMinimap::RegionMinimapCellData* first, unsigned int n, CRegionMinimap::RegionMinimapCellData const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAURegionMinimapCellData@CRegionMinimap@@IU12@@std@@YIPAURegionMinimapCellData@CRegionMinimap@@PAU12@IABU12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066E130, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0066e480 source (stdvector_Mfillinsert_0066e480.cpp) names CRegionMinimap::RegionMinimapCellData; binder ??$uninitialized_fill_n@PAURegionMinimapCellData@CRegionMinimap@@IU12@@std@@YIPAURegionMinimapCellData@CRegionMinimap@@PAU12@IABU12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CRegionMinimap::RegionMinimapCellData.
#include "stlport/fable_containers.h"
class CRegionMinimap {
public:
struct RegionMinimapCellData {
    long first;
    long second;
};
};
template CRegionMinimap::RegionMinimapCellData* std::uninitialized_fill_n<CRegionMinimap::RegionMinimapCellData*,unsigned int,CRegionMinimap::RegionMinimapCellData >(CRegionMinimap::RegionMinimapCellData*, unsigned int, CRegionMinimap::RegionMinimapCellData const&);
