// std::fill(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, CEngineSubPrimitiveRenderer::CSortElement const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCSortElement@CEngineSubPrimitiveRenderer@@V12@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84D60, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b85fd0 std::vector<CEngineSubPrimitiveRenderer::CSortElement, std::allocator<CEngineSubPrimitiveRenderer::CSortElement>>::_M_fill_insert; caller 00b85fd0 std::vector<CEngineSubPrimitiveRenderer::CSortElement, std::allocator<CEngineSubPrimitiveRenderer::CSortElement>>::_M_fill_insert; binder ??$fill@PAVCSortElement@CEngineSubPrimitiveRenderer@@V12@@std@@YIXPAVCSortElement@CEngineSubPrimitiveRenderer@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineSubPrimitiveRenderer::CSortElement.
#include "stlport/fable_containers.h"
class CEngineSubPrimitiveRenderer {
public:
class CSortElement {
public: unsigned char _body[0x8];
};
};
template void std::fill<CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement >(CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement const&);
