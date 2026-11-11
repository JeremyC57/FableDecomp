// std::fill(CCrimeWaitingForBribe* first, CCrimeWaitingForBribe* last, CCrimeWaitingForBribe const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCCrimeWaitingForBribe@@V1@@std@@YIXPAVCCrimeWaitingForBribe@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B3C80, 37 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 007b4e00 source (stdvector_Mfillinsert_007b4e00.cpp) names CCrimeWaitingForBribe; binder ??$fill@PAVCCrimeWaitingForBribe@@V1@@std@@YIXPAVCCrimeWaitingForBribe@@0ABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CCrimeWaitingForBribe.
#include "stlport/fable_containers.h"
class CCrimeWaitingForBribe {
public:
    unsigned char _body[0x8];
};
template void std::fill<CCrimeWaitingForBribe*,CCrimeWaitingForBribe >(CCrimeWaitingForBribe*, CCrimeWaitingForBribe*, CCrimeWaitingForBribe const&);
