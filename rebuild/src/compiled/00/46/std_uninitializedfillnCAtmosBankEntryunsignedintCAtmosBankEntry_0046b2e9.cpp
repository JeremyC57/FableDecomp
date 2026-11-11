// std::uninitialized_fill_n(CAtmosBankEntry* first, unsigned int n, CAtmosBankEntry const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCAtmosBankEntry@@IV1@@std@@YIPAVCAtmosBankEntry@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046B2E9, 35 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0046b1b8 std::vector<CAtmosBankEntry, std::allocator<CAtmosBankEntry>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCAtmosBankEntry@@IV1@@std@@YIPAVCAtmosBankEntry@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CAtmosBankEntry.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#include "stlport/fable_containers.h"
class CAtmosBankEntry {
public: long m_0; long m_4;
};
template CAtmosBankEntry* std::uninitialized_fill_n<CAtmosBankEntry*,unsigned int,CAtmosBankEntry >(CAtmosBankEntry*, unsigned int, CAtmosBankEntry const&);
