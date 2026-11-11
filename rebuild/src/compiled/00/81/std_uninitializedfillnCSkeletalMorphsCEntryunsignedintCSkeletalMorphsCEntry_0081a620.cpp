// std::uninitialized_fill_n(CSkeletalMorphs::CEntry* first, unsigned int n, CSkeletalMorphs::CEntry const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCEntry@CSkeletalMorphs@@IV12@@std@@YIPAVCEntry@CSkeletalMorphs@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081A620, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0081cf90 source (stdvector_Mfillinsert_0081cf90.cpp) names CSkeletalMorphs::CEntry; binder ??$uninitialized_fill_n@PAVCEntry@CSkeletalMorphs@@IV12@@std@@YIPAVCEntry@CSkeletalMorphs@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSkeletalMorphs::CEntry.
#include "stlport/fable_containers.h"
class CSkeletalMorphs {
public:
class CEntry {
public: long m_0; long m_4;
};
};
template CSkeletalMorphs::CEntry* std::uninitialized_fill_n<CSkeletalMorphs::CEntry*,unsigned int,CSkeletalMorphs::CEntry >(CSkeletalMorphs::CEntry*, unsigned int, CSkeletalMorphs::CEntry const&);
