// std::__uninitialized_fill_n(CCountedPointer<CTCLightningNode::CLightningBranch>* first, unsigned int n, CCountedPointer<CTCLightningNode::CLightningBranch> const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@IV1@@std@@YIPAV?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F3820, 46 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007f47f0 source (stdvector_Minsertoverflow_007f47f0.cpp) names CCountedPointer<CTCLightningNode::CLightningBranch>; binder ??$__uninitialized_fill_n@PAV?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@IV1@@std@@YIPAV?$CCountedPointer@VCLightningBranch@CTCLightningNode@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCountedPointer<CTCLightningNode::CLightningBranch>.
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
class CTCLightningNode {
public:
class CLightningBranch;
};
template CCountedPointer<CTCLightningNode::CLightningBranch>* std::__uninitialized_fill_n<CCountedPointer<CTCLightningNode::CLightningBranch>*,unsigned int,CCountedPointer<CTCLightningNode::CLightningBranch> >(CCountedPointer<CTCLightningNode::CLightningBranch>*, unsigned int, CCountedPointer<CTCLightningNode::CLightningBranch> const&, std::__false_type const&);
