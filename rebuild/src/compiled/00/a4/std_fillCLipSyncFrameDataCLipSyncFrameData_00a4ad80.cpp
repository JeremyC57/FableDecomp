// std::fill(CLipSyncFrameData* first, CLipSyncFrameData* last, CLipSyncFrameData const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCLipSyncFrameData@@V1@@std@@YIXPAVCLipSyncFrameData@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4AD80, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a4b020 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::_M_fill_insert; binder ??$fill@PAVCLipSyncFrameData@@V1@@std@@YIXPAVCLipSyncFrameData@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLipSyncFrameData.
#include "stlport/fable_containers.h"
class CLipSyncFrameData {
public:
    unsigned char _body[0xc];
    CLipSyncFrameData& operator=(const CLipSyncFrameData& other);   // 0x00A4A520
};
template void std::fill<CLipSyncFrameData*,CLipSyncFrameData >(CLipSyncFrameData*, CLipSyncFrameData*, CLipSyncFrameData const&);
