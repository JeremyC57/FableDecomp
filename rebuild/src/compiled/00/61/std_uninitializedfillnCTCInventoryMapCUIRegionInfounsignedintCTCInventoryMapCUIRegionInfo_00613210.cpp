// std::uninitialized_fill_n(CTCInventoryMap::CUIRegionInfo* first, unsigned int n, CTCInventoryMap::CUIRegionInfo const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCUIRegionInfo@CTCInventoryMap@@IV12@@std@@YIPAVCUIRegionInfo@CTCInventoryMap@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00613210, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00618990 source (stdvector_Mfillinsert_00618990.cpp) names CTCInventoryMap::CUIRegionInfo; binder ??$uninitialized_fill_n@PAVCUIRegionInfo@CTCInventoryMap@@IV12@@std@@YIPAVCUIRegionInfo@CTCInventoryMap@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTCInventoryMap::CUIRegionInfo.
#include "stlport/fable_containers.h"
class CTCInventoryMap {
public:
class CUIRegionInfo {
public: unsigned char _body[0x28];
};
};
template CTCInventoryMap::CUIRegionInfo* std::uninitialized_fill_n<CTCInventoryMap::CUIRegionInfo*,unsigned int,CTCInventoryMap::CUIRegionInfo >(CTCInventoryMap::CUIRegionInfo*, unsigned int, CTCInventoryMap::CUIRegionInfo const&);
