// std::fill(CComplexNumber* first, CComplexNumber* last, CComplexNumber const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCComplexNumber@@V1@@std@@YIXPAVCComplexNumber@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A482F0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00a4a3d0 std::vector<CComplexNumber, std::allocator<CComplexNumber>>::_M_fill_insert; caller 00a4a3d0 std::vector<CComplexNumber, std::allocator<CComplexNumber>>::_M_fill_insert; binder ??$fill@PAVCComplexNumber@@V1@@std@@YIXPAVCComplexNumber@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CComplexNumber.
#include "stlport/fable_containers.h"
class CComplexNumber {
public:
    unsigned char _body[0x8];
};
template void std::fill<CComplexNumber*,CComplexNumber >(CComplexNumber*, CComplexNumber*, CComplexNumber const&);
