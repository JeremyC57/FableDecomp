// std::__uninitialized_copy(CCrimeWaitingForBribe* first, CCrimeWaitingForBribe* last, CCrimeWaitingForBribe* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCCrimeWaitingForBribe@@PAV1@@std@@YIPAVCCrimeWaitingForBribe@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B41C0, 44 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 007b4e00 source (stdvector_Mfillinsert_007b4e00.cpp) names CCrimeWaitingForBribe; binder ??$__uninitialized_copy@PAVCCrimeWaitingForBribe@@PAV1@@std@@YIPAVCCrimeWaitingForBribe@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCrimeWaitingForBribe.
#include "stlport/fable_containers.h"
class CCrimeWaitingForBribe {
public:                        // dword members: the copy is memberwise (a byte array copies through pointer registers)
    long m_00;
    long m_04;
};
template CCrimeWaitingForBribe* std::__uninitialized_copy<CCrimeWaitingForBribe*,CCrimeWaitingForBribe* >(CCrimeWaitingForBribe*, CCrimeWaitingForBribe*, CCrimeWaitingForBribe*, std::__false_type const&);
