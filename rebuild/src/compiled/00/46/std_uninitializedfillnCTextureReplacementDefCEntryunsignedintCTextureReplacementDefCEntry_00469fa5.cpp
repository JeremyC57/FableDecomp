// std::uninitialized_fill_n(CTextureReplacementDef::CEntry* first, unsigned int n, CTextureReplacementDef::CEntry const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCEntry@CTextureReplacementDef@@IV12@@std@@YIPAVCEntry@CTextureReplacementDef@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00469FA5, 35 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 00469e74 std::vector<CTextureReplacementDef::CEntry, std::allocator<CTextureReplacementDef::CEntry>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCEntry@CTextureReplacementDef@@IV12@@std@@YIPAVCEntry@CTextureReplacementDef@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTextureReplacementDef::CEntry.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#include "stlport/fable_containers.h"
class CTextureReplacementDef {
public:
class CEntry {
public: long m_0; long m_4;
};
};
template CTextureReplacementDef::CEntry* std::uninitialized_fill_n<CTextureReplacementDef::CEntry*,unsigned int,CTextureReplacementDef::CEntry >(CTextureReplacementDef::CEntry*, unsigned int, CTextureReplacementDef::CEntry const&);
