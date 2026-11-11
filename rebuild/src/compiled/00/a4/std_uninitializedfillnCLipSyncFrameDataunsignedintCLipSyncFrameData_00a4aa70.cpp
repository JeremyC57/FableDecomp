// std::uninitialized_fill_n(CLipSyncFrameData* first, unsigned int n, CLipSyncFrameData const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCLipSyncFrameData@@IV1@@std@@YIPAVCLipSyncFrameData@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4AA70, 42 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a4b020 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCLipSyncFrameData@@IV1@@std@@YIPAVCLipSyncFrameData@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLipSyncFrameData.
#include "stlport/fable_containers.h"
class CLipSyncFrameData {
public:
    unsigned char _body[0xc];
    CLipSyncFrameData(const CLipSyncFrameData& other);           // 0x00A49C60
};
template CLipSyncFrameData* std::uninitialized_fill_n<CLipSyncFrameData*,unsigned int,CLipSyncFrameData >(CLipSyncFrameData*, unsigned int, CLipSyncFrameData const&);
