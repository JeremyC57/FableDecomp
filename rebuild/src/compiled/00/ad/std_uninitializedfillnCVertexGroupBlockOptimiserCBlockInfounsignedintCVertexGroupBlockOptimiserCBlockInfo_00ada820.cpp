// std::__uninitialized_fill_n(CVertexGroupBlockOptimiser::CBlockInfo* first, unsigned int n, CVertexGroupBlockOptimiser::CBlockInfo const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCBlockInfo@CVertexGroupBlockOptimiser@@IV12@@std@@YIPAVCBlockInfo@CVertexGroupBlockOptimiser@@PAV12@IABV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADA820, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00adbd50 std::vector<CVertexGroupBlockOptimiser::CBlockInfo, std::allocator<CVertexGroupBlockOptimiser::CBlockInfo>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAVCBlockInfo@CVertexGroupBlockOptimiser@@IV12@@std@@YIPAVCBlockInfo@CVertexGroupBlockOptimiser@@PAV12@IABV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CVertexGroupBlockOptimiser::CBlockInfo.
#include "stlport/fable_containers.h"
class CVertexGroupBlockOptimiser {
public:
class CBlockInfo {
public: long m_0; long m_4; long m_8;
};
};
template CVertexGroupBlockOptimiser::CBlockInfo* std::__uninitialized_fill_n<CVertexGroupBlockOptimiser::CBlockInfo*,unsigned int,CVertexGroupBlockOptimiser::CBlockInfo >(CVertexGroupBlockOptimiser::CBlockInfo*, unsigned int, CVertexGroupBlockOptimiser::CBlockInfo const&, std::__false_type const&);
