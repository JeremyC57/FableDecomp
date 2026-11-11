// std::__uninitialized_copy(CLipSyncFrameData* first, CLipSyncFrameData* last, CLipSyncFrameData* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCLipSyncFrameData@@PAV1@@std@@YIPAVCLipSyncFrameData@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4AA10, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a4af00 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::_M_insert_overflow; caller 00a4b020 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::_M_fill_insert; caller 00a4b020 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCLipSyncFrameData@@PAV1@@std@@YIPAVCLipSyncFrameData@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLipSyncFrameData.
#include "stlport/fable_containers.h"
class CLipSyncFrameData {
public:
    unsigned char _body[0xc];
    CLipSyncFrameData(const CLipSyncFrameData& other);           // 0x00A49C60
};
template CLipSyncFrameData* std::__uninitialized_copy<CLipSyncFrameData*,CLipSyncFrameData* >(CLipSyncFrameData*, CLipSyncFrameData*, CLipSyncFrameData*, std::__false_type const&);
