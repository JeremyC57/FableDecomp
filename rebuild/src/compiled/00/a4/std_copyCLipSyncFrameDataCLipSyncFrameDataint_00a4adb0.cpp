// std::__copy(CLipSyncFrameData* first, CLipSyncFrameData* last, CLipSyncFrameData* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCLipSyncFrameData@@PAV1@H@std@@YIPAVCLipSyncFrameData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4ADB0, 66 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a4b160 std::vector<CLipSyncFrameData, std::allocator<CLipSyncFrameData>>::resize; binder ??$__copy@PAVCLipSyncFrameData@@PAV1@H@std@@YIPAVCLipSyncFrameData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CLipSyncFrameData.
#include "stlport/fable_containers.h"
class CLipSyncFrameData {
public:
    unsigned char _body[0xc];
    CLipSyncFrameData& operator=(const CLipSyncFrameData& other);   // 0x00A4A520
};
template CLipSyncFrameData* std::__copy<CLipSyncFrameData*,CLipSyncFrameData*,int >(CLipSyncFrameData*, CLipSyncFrameData*, CLipSyncFrameData*, std::random_access_iterator_tag const&, int*);
