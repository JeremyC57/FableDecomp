// std::__uninitialized_fill_n(CSubtitleRenderer::CEmbeddedHighlight* first, unsigned int n, CSubtitleRenderer::CEmbeddedHighlight const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCEmbeddedHighlight@CSubtitleRenderer@@IV12@@std@@YIPAVCEmbeddedHighlight@CSubtitleRenderer@@PAV12@IABV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004939EA, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 004941f2 std::vector<CSubtitleRenderer::CEmbeddedHighlight, std::allocator<CSubtitleRenderer::CEmbeddedHighlight>>::_M_insert_overflow; callee 00493617 _unattributed::operator=; binder ??$__uninitialized_fill_n@PAVCEmbeddedHighlight@CSubtitleRenderer@@IV12@@std@@YIPAVCEmbeddedHighlight@CSubtitleRenderer@@PAV12@IABV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSubtitleRenderer::CEmbeddedHighlight.
#pragma optimize("s",on)
#include "stlport/fable_containers.h"
class CSubtitleRenderer {
public:
class CEmbeddedHighlight {
public:
    unsigned char _body[0x14];
    CEmbeddedHighlight(const CEmbeddedHighlight& other);           // 0x00493617
};
};
template CSubtitleRenderer::CEmbeddedHighlight* std::__uninitialized_fill_n<CSubtitleRenderer::CEmbeddedHighlight*,unsigned int,CSubtitleRenderer::CEmbeddedHighlight >(CSubtitleRenderer::CEmbeddedHighlight*, unsigned int, CSubtitleRenderer::CEmbeddedHighlight const&, std::__false_type const&);
