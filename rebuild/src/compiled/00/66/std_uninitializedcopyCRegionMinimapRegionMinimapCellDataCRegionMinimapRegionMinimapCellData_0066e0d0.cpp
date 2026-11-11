// std::__uninitialized_copy(CRegionMinimap::RegionMinimapCellData* first, CRegionMinimap::RegionMinimapCellData* last, CRegionMinimap::RegionMinimapCellData* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAURegionMinimapCellData@CRegionMinimap@@PAU12@@std@@YIPAURegionMinimapCellData@CRegionMinimap@@PAU12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066E0D0, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0066e480 source (stdvector_Mfillinsert_0066e480.cpp) names CRegionMinimap::RegionMinimapCellData; binder ??$__uninitialized_copy@PAURegionMinimapCellData@CRegionMinimap@@PAU12@@std@@YIPAURegionMinimapCellData@CRegionMinimap@@PAU12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CRegionMinimap::RegionMinimapCellData.
#include "stlport/fable_containers.h"
class CRegionMinimap {
public:
struct RegionMinimapCellData {                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};
};
template CRegionMinimap::RegionMinimapCellData* std::__uninitialized_copy<CRegionMinimap::RegionMinimapCellData*,CRegionMinimap::RegionMinimapCellData* >(CRegionMinimap::RegionMinimapCellData*, CRegionMinimap::RegionMinimapCellData*, CRegionMinimap::RegionMinimapCellData*, std::__false_type const&);
