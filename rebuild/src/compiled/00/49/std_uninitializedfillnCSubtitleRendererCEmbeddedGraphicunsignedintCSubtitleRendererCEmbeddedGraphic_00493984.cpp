// std::__uninitialized_fill_n(CSubtitleRenderer::CEmbeddedGraphic* first, unsigned int n, CSubtitleRenderer::CEmbeddedGraphic const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCEmbeddedGraphic@CSubtitleRenderer@@IV12@@std@@YIPAVCEmbeddedGraphic@CSubtitleRenderer@@PAV12@IABV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493984, 38 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00493ff8 std::vector<CSubtitleRenderer::CEmbeddedGraphic>::_M_insert_overflow; callee 0x004935F8 ??0CEmbeddedGraphic@CSubtitleRenderer@@QAE@ABV01@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSubtitleRenderer::CEmbeddedGraphic.
#pragma optimize("s",on)
#include "stlport/fable_containers.h"
class CSubtitleRenderer {
public:
class CEmbeddedGraphic {
public:
    unsigned char _body[0x10];
    CEmbeddedGraphic(const CEmbeddedGraphic& other);           // 0x004935F8
};
};
template CSubtitleRenderer::CEmbeddedGraphic* std::__uninitialized_fill_n<CSubtitleRenderer::CEmbeddedGraphic*,unsigned int,CSubtitleRenderer::CEmbeddedGraphic >(CSubtitleRenderer::CEmbeddedGraphic*, unsigned int, CSubtitleRenderer::CEmbeddedGraphic const&, std::__false_type const&);
