// std::__uninitialized_copy(CSubtitleRenderer::CEmbeddedGraphic* first, CSubtitleRenderer::CEmbeddedGraphic* last, CSubtitleRenderer::CEmbeddedGraphic* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCEmbeddedGraphic@CSubtitleRenderer@@PAV12@@std@@YIPAVCEmbeddedGraphic@CSubtitleRenderer@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00493959, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00493ff8 std::vector<CSubtitleRenderer::CEmbeddedGraphic>::_M_insert_overflow (non-const iterators: _M_start / position); callee 0x004935F8 ??0CEmbeddedGraphic@CSubtitleRenderer@@QAE@ABV01@@Z
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
template CSubtitleRenderer::CEmbeddedGraphic* std::__uninitialized_copy<CSubtitleRenderer::CEmbeddedGraphic*,CSubtitleRenderer::CEmbeddedGraphic* >(CSubtitleRenderer::CEmbeddedGraphic*, CSubtitleRenderer::CEmbeddedGraphic*, CSubtitleRenderer::CEmbeddedGraphic*, std::__false_type const&);
