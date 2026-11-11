// std::__uninitialized_fill_n(CBankFile::CRuntimeData* first, unsigned int n, CBankFile::CRuntimeData const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCRuntimeData@CBankFile@@IV12@@std@@YIPAVCRuntimeData@CBankFile@@PAV12@IABV12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D1CA0, 52 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 009d3580 std::vector<CBankFile::CRuntimeData, std::allocator<CBankFile::CRuntimeData>>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAVCRuntimeData@CBankFile@@IV12@@std@@YIPAVCRuntimeData@CBankFile@@PAV12@IABV12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CBankFile::CRuntimeData.
#include "stlport/fable_containers.h"
class CBankFile {
public:
class CRuntimeData {
public: long m_0; long m_4; long m_8;
};
};
template CBankFile::CRuntimeData* std::__uninitialized_fill_n<CBankFile::CRuntimeData*,unsigned int,CBankFile::CRuntimeData >(CBankFile::CRuntimeData*, unsigned int, CBankFile::CRuntimeData const&, std::__false_type const&);
