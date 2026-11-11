// std::fill(CParticleMorphs::CEntry* first, CParticleMorphs::CEntry* last, CParticleMorphs::CEntry const& x) -- STLport 4.6 _algobase.h: assign x to every element of [first, last)
// Leaf: ??$fill@PAVCEntry@CParticleMorphs@@V12@@std@@YIXPAVCEntry@CParticleMorphs@@0ABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00819360, 40 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0081d200 source (stdvector_Mfillinsert_0081d200.cpp) names CParticleMorphs::CEntry; binder ??$fill@PAVCEntry@CParticleMorphs@@V12@@std@@YIXPAVCEntry@CParticleMorphs@@0ABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CParticleMorphs::CEntry.
#include "stlport/fable_containers.h"
class CParticleMorphs {
public:
class CEntry {
public:
    unsigned char _body[0x24];
};
};
template void std::fill<CParticleMorphs::CEntry*,CParticleMorphs::CEntry >(CParticleMorphs::CEntry*, CParticleMorphs::CEntry*, CParticleMorphs::CEntry const&);
