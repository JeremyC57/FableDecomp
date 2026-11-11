// std::fill(CSkeletalMorphs::CEntry* first, CSkeletalMorphs::CEntry* last, CSkeletalMorphs::CEntry const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCEntry@CSkeletalMorphs@@V12@@std@@YIXPAVCEntry@CSkeletalMorphs@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x008192F0, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0081cf90 source (stdvector_Mfillinsert_0081cf90.cpp) names CSkeletalMorphs::CEntry; binder ??$fill@PAVCEntry@CSkeletalMorphs@@V12@@std@@YIXPAVCEntry@CSkeletalMorphs@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSkeletalMorphs::CEntry.
#include "stlport/fable_containers.h"
class CSkeletalMorphs {
public:
class CEntry {
public: unsigned char _body[0x8];
};
};
template void std::fill<CSkeletalMorphs::CEntry*,CSkeletalMorphs::CEntry >(CSkeletalMorphs::CEntry*, CSkeletalMorphs::CEntry*, CSkeletalMorphs::CEntry const&);
