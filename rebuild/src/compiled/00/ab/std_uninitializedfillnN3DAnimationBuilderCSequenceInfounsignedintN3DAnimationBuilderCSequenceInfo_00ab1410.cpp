// std::uninitialized_fill_n(N3DAnimationBuilder::CSequenceInfo* first, unsigned int n, N3DAnimationBuilder::CSequenceInfo const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCSequenceInfo@N3DAnimationBuilder@@IV12@@std@@YIPAVCSequenceInfo@N3DAnimationBuilder@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB1410, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00ab2e40 std::vector<N3DAnimationBuilder::CSequenceInfo, std::allocator<N3DAnimationBuilder::CSequenceInfo>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCSequenceInfo@N3DAnimationBuilder@@IV12@@std@@YIPAVCSequenceInfo@N3DAnimationBuilder@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over N3DAnimationBuilder::CSequenceInfo.
#include "stlport/fable_containers.h"
namespace N3DAnimationBuilder {
class CSequenceInfo {
public: unsigned char _body[0x34];
};
}
template N3DAnimationBuilder::CSequenceInfo* std::uninitialized_fill_n<N3DAnimationBuilder::CSequenceInfo*,unsigned int,N3DAnimationBuilder::CSequenceInfo >(N3DAnimationBuilder::CSequenceInfo*, unsigned int, N3DAnimationBuilder::CSequenceInfo const&);
