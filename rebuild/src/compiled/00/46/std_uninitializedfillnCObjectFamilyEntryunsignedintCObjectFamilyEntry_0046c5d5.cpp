// std::uninitialized_fill_n(CObjectFamilyEntry* first, unsigned int n, CObjectFamilyEntry const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCObjectFamilyEntry@@IV1@@std@@YIPAVCObjectFamilyEntry@@PAV1@IABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046C5D5, 35 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 0046c4a4 std::vector<CObjectFamilyEntry, std::allocator<CObjectFamilyEntry>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCObjectFamilyEntry@@IV1@@std@@YIPAVCObjectFamilyEntry@@PAV1@IABV1@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CObjectFamilyEntry.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_FORCEINLINE
#include "stlport/fable_containers.h"
class CObjectFamilyEntry {
public: long m_0; long m_4;
};
template CObjectFamilyEntry* std::uninitialized_fill_n<CObjectFamilyEntry*,unsigned int,CObjectFamilyEntry >(CObjectFamilyEntry*, unsigned int, CObjectFamilyEntry const&);
