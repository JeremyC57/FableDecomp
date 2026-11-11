// std::fill(N3DAnimationBuilder::CSequenceInfo* first, N3DAnimationBuilder::CSequenceInfo* last, N3DAnimationBuilder::CSequenceInfo const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCSequenceInfo@N3DAnimationBuilder@@V12@@std@@YIXPAVCSequenceInfo@N3DAnimationBuilder@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB10F0, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ab2e40 std::vector<N3DAnimationBuilder::CSequenceInfo, std::allocator<N3DAnimationBuilder::CSequenceInfo>>::_M_fill_insert; caller 00ab2e40 std::vector<N3DAnimationBuilder::CSequenceInfo, std::allocator<N3DAnimationBuilder::CSequenceInfo>>::_M_fill_insert; binder ??$fill@PAVCSequenceInfo@N3DAnimationBuilder@@V12@@std@@YIXPAVCSequenceInfo@N3DAnimationBuilder@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over N3DAnimationBuilder::CSequenceInfo.
#include "stlport/fable_containers.h"
namespace N3DAnimationBuilder {
class CSequenceInfo {
public:
    unsigned char _body[0x34];
};
}
template void std::fill<N3DAnimationBuilder::CSequenceInfo*,N3DAnimationBuilder::CSequenceInfo >(N3DAnimationBuilder::CSequenceInfo*, N3DAnimationBuilder::CSequenceInfo*, N3DAnimationBuilder::CSequenceInfo const&);
