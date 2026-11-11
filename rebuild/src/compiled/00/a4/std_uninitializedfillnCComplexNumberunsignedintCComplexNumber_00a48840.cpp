// std::uninitialized_fill_n(CComplexNumber* first, unsigned int n, CComplexNumber const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCComplexNumber@@IV1@@std@@YIPAVCComplexNumber@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A48840, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a4a3d0 std::vector<CComplexNumber, std::allocator<CComplexNumber>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCComplexNumber@@IV1@@std@@YIPAVCComplexNumber@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CComplexNumber.
#include "stlport/fable_containers.h"
class CComplexNumber {
public: long m_0; long m_4;
};
template CComplexNumber* std::uninitialized_fill_n<CComplexNumber*,unsigned int,CComplexNumber >(CComplexNumber*, unsigned int, CComplexNumber const&);
