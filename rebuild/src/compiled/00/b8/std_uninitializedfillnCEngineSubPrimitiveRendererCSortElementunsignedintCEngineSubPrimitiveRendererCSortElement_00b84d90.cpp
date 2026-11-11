// std::uninitialized_fill_n(CEngineSubPrimitiveRenderer::CSortElement* first, unsigned int n, CEngineSubPrimitiveRenderer::CSortElement const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCSortElement@CEngineSubPrimitiveRenderer@@IV12@@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84D90, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00b85fd0 std::vector<CEngineSubPrimitiveRenderer::CSortElement, std::allocator<CEngineSubPrimitiveRenderer::CSortElement>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCSortElement@CEngineSubPrimitiveRenderer@@IV12@@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineSubPrimitiveRenderer::CSortElement.
#include "stlport/fable_containers.h"
class CEngineSubPrimitiveRenderer {
public:
class CSortElement {
public:
    long first;
    long second;
};
};
template CEngineSubPrimitiveRenderer::CSortElement* std::uninitialized_fill_n<CEngineSubPrimitiveRenderer::CSortElement*,unsigned int,CEngineSubPrimitiveRenderer::CSortElement >(CEngineSubPrimitiveRenderer::CSortElement*, unsigned int, CEngineSubPrimitiveRenderer::CSortElement const&);
