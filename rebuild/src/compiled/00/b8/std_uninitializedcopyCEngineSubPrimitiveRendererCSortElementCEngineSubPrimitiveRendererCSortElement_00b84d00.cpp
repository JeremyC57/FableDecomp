// std::__uninitialized_copy(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, CEngineSubPrimitiveRenderer::CSortElement* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84D00, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00b85fd0 std::vector<CEngineSubPrimitiveRenderer::CSortElement, std::allocator<CEngineSubPrimitiveRenderer::CSortElement>>::_M_fill_insert; caller 00b85fd0 std::vector<CEngineSubPrimitiveRenderer::CSortElement, std::allocator<CEngineSubPrimitiveRenderer::CSortElement>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineSubPrimitiveRenderer::CSortElement.
#include "stlport/fable_containers.h"
class CEngineSubPrimitiveRenderer {
public:
class CSortElement {
public:                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};
};
template CEngineSubPrimitiveRenderer::CSortElement* std::__uninitialized_copy<CEngineSubPrimitiveRenderer::CSortElement*,CEngineSubPrimitiveRenderer::CSortElement* >(CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*, CEngineSubPrimitiveRenderer::CSortElement*, std::__false_type const&);
