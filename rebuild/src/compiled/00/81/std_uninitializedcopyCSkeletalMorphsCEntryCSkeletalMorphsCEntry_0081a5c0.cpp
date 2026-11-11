// std::__uninitialized_copy(CSkeletalMorphs::CEntry* first, CSkeletalMorphs::CEntry* last, CSkeletalMorphs::CEntry* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCEntry@CSkeletalMorphs@@PAV12@@std@@YIPAVCEntry@CSkeletalMorphs@@PAV12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081A5C0, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0081cf90 source (stdvector_Mfillinsert_0081cf90.cpp) names CSkeletalMorphs::CEntry; binder ??$__uninitialized_copy@PAVCEntry@CSkeletalMorphs@@PAV12@@std@@YIPAVCEntry@CSkeletalMorphs@@PAV12@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CSkeletalMorphs::CEntry.
#include "stlport/fable_containers.h"
class CSkeletalMorphs {
public:
class CEntry {
public:                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};
};
template CSkeletalMorphs::CEntry* std::__uninitialized_copy<CSkeletalMorphs::CEntry*,CSkeletalMorphs::CEntry* >(CSkeletalMorphs::CEntry*, CSkeletalMorphs::CEntry*, CSkeletalMorphs::CEntry*, std::__false_type const&);
