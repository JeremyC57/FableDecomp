// std::__uninitialized_copy(CTextureManagerPoolInfo* first, CTextureManagerPoolInfo* last, CTextureManagerPoolInfo* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAVCTextureManagerPoolInfo@@PAV1@@std@@YIPAVCTextureManagerPoolInfo@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085DB50, 62 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 0085e200 std::vector<CTextureManagerPoolInfo>::_M_insert_overflow; caller 0085e200 std::vector<CTextureManagerPoolInfo>::_M_insert_overflow; binder ??$__uninitialized_copy@PAVCTextureManagerPoolInfo@@PAV1@@std@@YIPAVCTextureManagerPoolInfo@@PAV1@00ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CTextureManagerPoolInfo.
#include "stlport/fable_containers.h"
class CTextureManagerPoolInfo {
public:                              // 20-byte record with an explicit memberwise copy constructor
    long m_0, m_4, m_8, m_c, m_10;
    CTextureManagerPoolInfo(const CTextureManagerPoolInfo& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_c(o.m_c), m_10(o.m_10) { }
};
template CTextureManagerPoolInfo* std::__uninitialized_copy<CTextureManagerPoolInfo*,CTextureManagerPoolInfo* >(CTextureManagerPoolInfo*, CTextureManagerPoolInfo*, CTextureManagerPoolInfo*, std::__false_type const&);
