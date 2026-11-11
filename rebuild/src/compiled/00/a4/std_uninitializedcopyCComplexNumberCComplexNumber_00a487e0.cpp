// std::__uninitialized_copy(CComplexNumber* first, CComplexNumber* last, CComplexNumber* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCComplexNumber@@PAV1@@std@@YIPAVCComplexNumber@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A487E0, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00a4a3d0 std::vector<CComplexNumber, std::allocator<CComplexNumber>>::_M_fill_insert; caller 00a4a3d0 std::vector<CComplexNumber, std::allocator<CComplexNumber>>::_M_fill_insert; binder ??$__uninitialized_copy@PAVCComplexNumber@@PAV1@@std@@YIPAVCComplexNumber@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CComplexNumber.
#include "stlport/fable_containers.h"
class CComplexNumber {
public:                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};
template CComplexNumber* std::__uninitialized_copy<CComplexNumber*,CComplexNumber* >(CComplexNumber*, CComplexNumber*, CComplexNumber*, std::__false_type const&);
