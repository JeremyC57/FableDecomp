// std::fill(CTCInventoryMap::CUIRegionInfo* first, CTCInventoryMap::CUIRegionInfo* last, CTCInventoryMap::CUIRegionInfo const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCUIRegionInfo@CTCInventoryMap@@V12@@std@@YIXPAVCUIRegionInfo@CTCInventoryMap@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x006131E0, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00618990 source (stdvector_Mfillinsert_00618990.cpp) names CTCInventoryMap::CUIRegionInfo; binder ??$fill@PAVCUIRegionInfo@CTCInventoryMap@@V12@@std@@YIXPAVCUIRegionInfo@CTCInventoryMap@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTCInventoryMap::CUIRegionInfo.
#include "stlport/fable_containers.h"
class CTCInventoryMap {
public:
class CUIRegionInfo {
public:
    unsigned char _body[0x28];
};
};
template void std::fill<CTCInventoryMap::CUIRegionInfo*,CTCInventoryMap::CUIRegionInfo >(CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo const&);
