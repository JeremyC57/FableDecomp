// std::fill(NGraphicAppearance::CPrimitive** first, NGraphicAppearance::CPrimitive** last, NGraphicAppearance::CPrimitive* const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAPAVCPrimitive@NGraphicAppearance@@PAV12@@std@@YIXPAPAVCPrimitive@NGraphicAppearance@@0ABQAV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00640A70, 31 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00642440 std::vector<NGraphicAppearance::CPrimitive *, std::allocator<NGraphicAppearance::CPrimitive *>>::_M_fill_insert; caller 00642440 std::vector<NGraphicAppearance::CPrimitive *, std::allocator<NGraphicAppearance::CPrimitive *>>::_M_fill_insert; binder ??$fill@PAPAVCPrimitive@NGraphicAppearance@@PAV12@@std@@YIXPAPAVCPrimitive@NGraphicAppearance@@0ABQAV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NGraphicAppearance::CPrimitive*.
#include "stlport/fable_containers.h"
namespace NGraphicAppearance {
class CPrimitive;
}
template void std::fill<NGraphicAppearance::CPrimitive**,NGraphicAppearance::CPrimitive* >(NGraphicAppearance::CPrimitive**, NGraphicAppearance::CPrimitive**, NGraphicAppearance::CPrimitive* const&);
