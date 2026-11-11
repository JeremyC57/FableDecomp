// std::uninitialized_fill_n(CCrimeWaitingForBribe* first, unsigned int n, CCrimeWaitingForBribe const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCCrimeWaitingForBribe@@IV1@@std@@YIPAVCCrimeWaitingForBribe@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B4220, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 007b4e00 source (stdvector_Mfillinsert_007b4e00.cpp) names CCrimeWaitingForBribe; binder ??$uninitialized_fill_n@PAVCCrimeWaitingForBribe@@IV1@@std@@YIPAVCCrimeWaitingForBribe@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCrimeWaitingForBribe.
#include "stlport/fable_containers.h"
class CCrimeWaitingForBribe {
public:
    long first;
    long second;
};
template CCrimeWaitingForBribe* std::uninitialized_fill_n<CCrimeWaitingForBribe*,unsigned int,CCrimeWaitingForBribe >(CCrimeWaitingForBribe*, unsigned int, CCrimeWaitingForBribe const&);
