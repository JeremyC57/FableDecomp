// std::__copy_backward(CLipSyncFrameData* first, CLipSyncFrameData* last, CLipSyncFrameData* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVCLipSyncFrameData@@PAV1@H@std@@YIPAVCLipSyncFrameData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4AAC0, 66 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a4b020 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::_M_fill_insert; binder ??$__copy_backward@PAVCLipSyncFrameData@@PAV1@H@std@@YIPAVCLipSyncFrameData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLipSyncFrameData.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
class CLipSyncFrameData {
public:
    unsigned char _body[0xc];
    CLipSyncFrameData& operator=(const CLipSyncFrameData& other);   // 0x00A4A520
};
template CLipSyncFrameData* std::__copy_backward<CLipSyncFrameData*,CLipSyncFrameData*,int >(CLipSyncFrameData*, CLipSyncFrameData*, CLipSyncFrameData*, std::random_access_iterator_tag const&, int*);
