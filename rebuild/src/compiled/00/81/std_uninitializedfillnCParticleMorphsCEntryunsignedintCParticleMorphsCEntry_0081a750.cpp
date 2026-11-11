// std::uninitialized_fill_n(CParticleMorphs::CEntry* first, unsigned int n, CParticleMorphs::CEntry const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCEntry@CParticleMorphs@@IV12@@std@@YIPAVCEntry@CParticleMorphs@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081A750, 43 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0081d200 source (stdvector_Mfillinsert_0081d200.cpp) names CParticleMorphs::CEntry; binder ??$uninitialized_fill_n@PAVCEntry@CParticleMorphs@@IV12@@std@@YIPAVCEntry@CParticleMorphs@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CParticleMorphs::CEntry.
#include "stlport/fable_containers.h"
class CParticleMorphs {
public:
class CEntry {
public: unsigned char _body[0x24];
};
};
template CParticleMorphs::CEntry* std::uninitialized_fill_n<CParticleMorphs::CEntry*,unsigned int,CParticleMorphs::CEntry >(CParticleMorphs::CEntry*, unsigned int, CParticleMorphs::CEntry const&);
